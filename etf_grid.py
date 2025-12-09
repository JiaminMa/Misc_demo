import backtrader as bt
import math
import os
import datetime
import sys

class CustomCSVData(bt.feeds.GenericCSVData):
    params = (
        ('dtformat', '%Y-%m-%d'),
        ('datetime', 0),
        ('close', 4),       # 收盘价列索引
        ('rounddecimals', 3),  # 自定义精度
    )

class ETFRecord():
    
    def __init__(self):
        self.base_price = 0
        self.last_price = 0
        self.position_cnt = 0 
        self.grid = 0
        self.name = ''
    
    def dump(self):
        name = self.name.split('/')[2].replace('.csv', '')
        print(f'======ETF {name}, base_price:{self.base_price}, last_price:{self.last_price}, cnt:{self.position_cnt}=====')
        
        
        
class ETFPortfolioStrategy(bt.Strategy):
    params = (
        ('num_etfs', 15),          # Number of ETFs in portfolio
        ('initial_position', 4000),# Initial investment per ETF
        ('position_limit', 40000), # Max position per ETF
        ('rebalance_pct', 0.02),   # 4% price change threshold
        ('batch_size', 100)        # Minimum shares per transaction
    )

    def __init__(self):
        self.etf_records = {}  # Track each ETF's status
        for d in self.datas:
            etf_record = ETFRecord()
            etf_record.name = d._name
            self.etf_records[d] = etf_record

    def next(self):
        # Main strategy logic on each trading day
        for i, d in enumerate(self.datas):
            etf_record = self.etf_records[d]
            if (len(d) == 1):
                self.execute_buy(d)
                etf_record.base_price = d.close[0]
                etf_record.grid = d.close[4] * self.params.rebalance_pct
                continue
                
            # self.log(f'close:{d.close[0]:.3f}')
            current_price = d.close[0]
            buy_price = etf_record.last_price - etf_record.grid
            sell_price = etf_record.last_price + etf_record.grid
            # self.log(f'####cur:{current_price:.3f}, buy:{buy_price:.3f}, sell:{sell_price:.3f}')
            
            if (current_price < buy_price):
                self.execute_buy(d)
                continue
    
            if (current_price > sell_price) and  (etf_record.position_cnt > 0):
                self.execute_sell(d)
                
                
    def execute_buy(self, data):
        volumn = self.params.initial_position / data.close[0]
        volumn = volumn - volumn % 100
        self.log(f'Buy {data._name} price:{data.close[0]}, vol:{volumn}')
        self.buy(data=data, size=volumn)
    
    def execute_sell(self, data):
        volumn = self.params.initial_position / data.close[0]
        volumn = volumn - volumn % 100
        self.log(f'Sell {data._name} price:{data.close[0]},  vol:{volumn}')
        self.sell(data=data, size=volumn)

    def log(self, txt):
        dt = self.datas[0].datetime.date(0)
        print(f'{dt.isoformat()}, {txt}')
        
    def notify_order(self, order):
        if order.status in [order.Submitted, order.Accepted]:
            return
        dn = order.data._name
        if order.status == order.Completed:
            etf_record = self.etf_records[order.data]
            if order.isbuy():
                self.log(f'Buy Done {dn}, price: {order.executed.price}, total:{order.executed.value}, comm:{order.executed.comm}')
                etf_record.position_cnt = etf_record.position_cnt + 1
            elif order.issell():
                self.log(f'Sell Done {dn}, price: {order.executed.price}, total:{order.executed.value}, comm:{order.executed.comm}')
                etf_record.position_cnt = etf_record.position_cnt - 1
            etf_record.last_price = order.executed.price
            etf_record.dump()

    def notify_trade(self, trade): 
        if not trade.isclosed: 
            return 
        dn = trade.data._name
        self.log(f'Profit {dn}, GP {trade.pnl}, NP {trade.pnlcomm}')



if __name__ == '__main__':
    
    # year = int(sys.argv[1])
    # Add your 15 ETF data feeds here (example format)
    # for etf_symbol in etf_list:
    #     data = bt.feeds.PandasData(dataname=your_etf_data)
    #     cerebro.adddata(data, name=etf_symbol)
    stock_data = 'etf_stock_data'
    stock_symbols = []
    for sym in os.listdir(stock_data):
        stock_symbols.append(f'./{stock_data}/{sym}')

    cash = 200000.00
    start_cash = cash
    for year in range(2020, 2026):
        cerebro = bt.Cerebro()
        cerebro.broker.setcash(cash)
        cerebro.broker.setcommission(commission=(1 / 10000))
        for sym in stock_symbols:
            data = CustomCSVData(
                dataname=sym,
                fromdate = datetime.datetime(year, 1, 1), 
                todate = datetime.datetime(year, 12, 31),
                reverse=False) 
            cerebro.adddata(data, name=sym)
        
        
        cerebro.addstrategy(ETFPortfolioStrategy)
        print(f'{year} start: {cerebro.broker.getvalue():.2f}')
        start = cerebro.broker.getvalue()
        cerebro.run()
        # print(f'End: {cerebro.broker.getvalue():.2f}')
        end = cerebro.broker.getvalue()
        print(f'{year} profit:{(end / start -  1) * 100:.2f}%')
        cash = end
    print(f'total profit:{(cash/start_cash - 1) * 100: .2f}%')
    # cerebro.plot()