import datetime # 
import os.path # 路径管理  
import sys # 获取当前运行脚本的路径 (in argv[0]) 
#导入backtrader框架  
import backtrader as bt 


LOG_EN = True

class CustomCSVData(bt.feeds.GenericCSVData):
    params = (
        ('dtformat', '%Y-%m-%d'),
        ('datetime', 0),
        ('close', 4),       # 收盘价列索引
        ('rounddecimals', 3),  # 自定义精度
    )

class ETFRecord():

    CASH = 400000.0
    BUY_COUNT = 9
    RESET_DAYS = 0
    SELL_RATE = 0.05

    def __init__(self):
        self.total = 0
        self.volumn = 0
        self.last_price = 0
        self.base_price = 0
        self.cost = 0
        self.m_per_buy = 0
        self.buy_count = 0
        self.sell_price = 0
        self.next_buy_price = 0
        self.reset_days = 0
        self.log = None

    def buy(self, price, volumn):
        self.cost = (self.cost * self.volumn + price * volumn) / (self.volumn + volumn)
        self.volumn = self.volumn + volumn
        self.buy_count = self.buy_count + 1
        self.next_buy_price = self.base_price * (1 - 0.01 * self.buy_count)
        self.sell_price = self.cost * (1 + ETFRecord.SELL_RATE)    

    def dump_profit(self, date, cur_price):
        self.log(f'cost:{self.cost:.3f}, profit_ratio{cur_price / self.cost:.3f}, cnt:{self.buy_count}', indictaor='    ')

    def set_log(self, log):
        self.log = log

    def set_limit(self, total):
        self.m_per_buy =  total / ETFRecord.BUY_COUNT

    def set_base_price(self, price):
        self.base_price = price

    def close(self):
        self.cost = 0
        self.volumn = 0
        self.buy_count = 0
        self.next_buy_price = 0
        self.sell_price = 0
        self.reset_days = ETFRecord.RESET_DAYS
    
    def tick(self):
        if (self.reset_days > 0):
            self.reset_days = self.reset_days - 1

    def can_buy(self):
        return self.reset_days == 0 and self.buy_count == 0

class TestStrategy(bt.Strategy): 
    params = ( 
            ('exitbars', 5), 
    )
    def log(self, txt, dt=None, indictaor=''): 
        dt = dt or self.datas[0].datetime.date(0)
        if LOG_EN == True:
            print(f'{indictaor}{dt.isoformat()}, {txt}') 
        
    def __init__(self): 
        self.dataclose = self.datas[0].close 
        self.order = None
        self.buyprice = None 
        self.buycomm = None
        self.etf_record = ETFRecord()
        self.etf_record.set_limit(ETFRecord.CASH)
        self.etf_record.set_log(self.log)

    def notify_order(self, order):
        if order.status in [order.Submitted, order.Accepted]:
            return
        dn = order.data._name
        if order.status == order.Completed:
            etf_record = self.etf_record
            if order.isbuy():
                self.log(f'Buy Done {dn}, price: {order.executed.price}, total:{order.executed.value}, comm:{order.executed.comm}', indictaor='    ')
                etf_record.buy(order.executed.price, order.executed.size)
                etf_record.dump_profit(self.datas[0].datetime.date(0), self.dataclose[0])
            elif order.issell():
                self.log(f'Sell Done {dn}, price: {order.executed.price}, total:{order.executed.value}, comm:{order.executed.comm}', indictaor='    ')
                etf_record.dump_profit(self.datas[0].datetime.date(0), self.dataclose[0])
                etf_record.close()
            etf_record.last_price = order.executed.price
        elif order.status in [order.Canceled, order.Margin, order.Rejected]: 
            self.log('Cancelled Money Not Enough',indictaor='    ') 
            self.order = None 


    def notify_trade(self, trade): 
        if not trade.isclosed: 
            return 
        dn = trade.data._name
        self.log(f'Profit {dn}, GP {trade.pnl:.3f}, NP {trade.pnlcomm:.3f}， CASH:{self.broker.get_cash():.3f}', indictaor='    ')


    def next(self): 
        # self.log(f'{self.dataclose[0]:.3f} {self.data._name}') 
        self.etf_record.tick()

        if self.order: 
            return 

        cur_price = self.dataclose[0]
        if ((len(self.datas[0]) == 1)) or (self.etf_record.can_buy() == True):
            self.execute_buy()
            self.etf_record.set_base_price(cur_price)
            return

        if (cur_price < self.etf_record.next_buy_price):
            self.execute_buy()
        elif (cur_price > self.etf_record.sell_price):
            self.close()
 
    def execute_buy(self):
        price = self.dataclose[0]
        volumn = self.etf_record.m_per_buy / price
        volumn = volumn - volumn % 100
        self.log(f'Buy {self.datas[0]._name} price:{price}, vol:{volumn}', indictaor='    ')
        self.buy(size=volumn)
 

class BackRecord():

    def __init__(self, buy_count=0, sell_rate=0, start_year='2020-9-1', end_year='2025-5-20'):
        self.reset_days = 0
        self.buy_count = buy_count
        self.sell_rate = sell_rate

        start_date = start_year.split('-')
        end_date = end_year.split('-')
        self.start_string = start_year
        self.end_string = end_year
        self.start_year = int(start_date[0])
        self.start_month = int(start_date[1])
        self.start_day = int(start_date[2])
        self.end_year = int(end_date[0])
        self.end_month = int(end_date[1])
        self.end_day = int(end_date[2])
        self.profit_ratio = 0
        self.max_drawdown = 0
        self.sharpe_ratio = 0
        self.test_name = f'B{buy_count}_R{sell_rate}'
        self.cerebro = None

    def run(self):

        ETFRecord.BUY_COUNT = self.buy_count
        ETFRecord.SELL_RATE = self.sell_rate

        print(f'==========Test{self.start_year}/{self.start_month}/{self.start_day}-{self.end_year}/{self.end_month}/{self.end_day}=====================')
        print(f'Param: REST_DAY:{ETFRecord.RESET_DAYS}, BUY_COUNT:{ETFRecord.BUY_COUNT}, SELL_RATE:{ETFRecord.SELL_RATE}')
        cerebro = bt.Cerebro() 
        cerebro.addstrategy(TestStrategy)
        cerebro.addanalyzer(bt.analyzers.SharpeRatio, _name='sharpe', riskfreerate=0.025)
        cerebro.addanalyzer(bt.analyzers.DrawDown, _name='drawdown')

        datapath = 'etf_stock_data/510760.csv'
        print(datapath)

        data = CustomCSVData( 
            dataname=datapath, 
            fromdate=datetime.datetime(self.start_year, self.start_month, self.start_day), 
            todate=datetime.datetime(self.end_year, self.end_month, self.end_day),
            reverse=False) 

        cerebro.adddata(data)
        cerebro.broker.setcash(ETFRecord.CASH)
        cerebro.broker.setcommission(commission=1/10000)
        cerebro.broker.set_slippage_perc(perc=0.0005, slip_open=True)
        start = cerebro.broker.getvalue()
        results = cerebro.run() 
        strat = results[0]
        sharpe_ratio = strat.analyzers.sharpe.get_analysis()['sharperatio'] 
        if sharpe_ratio == None:
            sharpe_ratio = 0
        drawdown_analysis = strat.analyzers.drawdown.get_analysis()
        max_drawdown = drawdown_analysis['max']['drawdown']   # 最大回撤百分比
        if max_drawdown == None:
            max_drawdown = 0

        self.max_drawdown = max_drawdown
        self.sharpe_ratio = sharpe_ratio
        self.profit_ratio = (cerebro.broker.getvalue() / start - 1)*100
        print(f'start:{start:.2f} End: {cerebro.broker.getvalue():.2f} {(cerebro.broker.getvalue() / start - 1)*100:.2f}%, sharpe_ratio:{sharpe_ratio:.2f}, max_drawdown: {max_drawdown:.2f}%')
        print(f'=====================Round End=====================\n\n')
        self.cerebro = cerebro


    

if __name__ == '__main__':

    LOG_EN = False

    start_time_list = [
        '2020-9-1',
        '2021-1-1',
        '2021-4-1',
        '2021-7-1',
        '2021-9-1',
        '2022-1-1',
        '2022-4-1',
        '2022-7-1',
        '2022-9-1',
        '2023-1-1',
        '2023-4-1',
        '2023-7-1',
        '2023-9-1',
        '2024-1-1',
        '2024-4-1',
        '2024-7-1',
        '2024-9-1',
        '2025-1-1',
    ]

    # all_year_max = {}
    # for start_time in start_time_list:
    #     buy_count_list = [7, 6, 5, 7, 6, 5]
    #     sell_rate_list = [0.02, 0.02, 0.02, 0.03, 0.03, 0.03]
    #     name_dict = {}
    #     profit_list = []

    #     for i in range(len(buy_count_list)):
    #         b = BackRecord(buy_count=buy_count_list[i], sell_rate=sell_rate_list[i], start_year=start_time, end_year='2025-5-20')
    #         b.run()
    #         profit_list.append(b.profit_ratio)
    #         name_dict[b.profit_ratio] = b.test_name
    #     max_value = max(profit_list)
    #     name = name_dict[max_value]
    #     print(f'max guy is {name}')
    #     if name in all_year_max:
    #         all_year_max[name] = all_year_max[name] + 1
    #     else:
    #         all_year_max[name] = 1
    # print(all_year_max)
    LOG_EN = 1
    b = BackRecord(buy_count=7, sell_rate=0.03, start_year='2020-9-1', end_year='2025-5-20')
    b.run()
    b.cerebro.plot()
    