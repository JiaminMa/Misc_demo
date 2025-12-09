import baostock as bs
import akshare as ak
import pandas as pd
import os
import time

# 登录baostock获取成分股列表
def get_hs300_stocks():
    lg = bs.login()
    if lg.error_code != '0':
        raise Exception(f"登录失败: {lg.error_msg}")
    rs = bs.query_hs300_stocks()
    stock_list = []
    while (rs.error_code == '0') & rs.next():
        code = rs.get_row_data()[1]  # 提取股票代码（如600519.SH）
        stock_list.append(code)
    bs.logout()
    return stock_list

# 抓取并保存股票数据
def fetch_and_save_stock_data(code, folder):
    try:
        print(code)
        symbol = code.split('.')[1]  # 处理代码格式（如600519.SH → 600519）
        # 获取未复权数据（开盘、最高、最低、收盘、成交量）
        df_raw = ak.stock_zh_a_hist(symbol=symbol, period="daily", adjust="")
        df_raw = df_raw[['日期', '开盘', '最高', '最低', '收盘', '成交量']]
        # 获取后复权数据（调整后收盘价）
        df_adj = ak.stock_zh_a_hist(symbol=symbol, period="daily", adjust="hfq")
        df_adj = df_adj[['日期', '收盘']].rename(columns={'收盘': '复权收盘价'})
        # 合并数据
        merged_df = pd.merge(df_raw, df_adj, on='日期', how='left')
        # 重命名列并调整顺序
        merged_df.columns = ['Date', 'Open', 'High', 'Low', 'Close', 'Volume', 'Adj Close']
        merged_df = merged_df[['Date', 'Open', 'High', 'Low', 'Close', 'Adj Close', 'Volume']]
        # 保存为CSV
        filename = os.path.join(folder, f"{code.replace('.', '_')}_daily.csv")
        merged_df.to_csv(filename, index=False)
        print(f"已保存: {filename}")
    except Exception as e:
        print(f"处理股票 {code} 时出错: {e}")

def fetch_etf_and_save_stock(code, folder):
    df = ak.fund_etf_hist_em(
        symbol=code,  
        period="daily",   
        start_date="20200101",
        end_date="20251101",
        adjust="qfq" 
    )

    df = df.rename(columns={
        "日期": "Date",
        "开盘": "Open",
        "最高": "High",
        "最低": "Low",
        "收盘": "Close",
        "成交量": "Volume"
    })

    df["Adj Close"] = df["Close"]

    # 按指定顺序保存为 CSV
    df[["Date", "Open", "High", "Low", "Close", "Adj Close", "Volume"]].to_csv(
        f'{folder}/{code}.csv', 
        index=False,
        encoding="utf-8-sig"
    )

# 主程序
if __name__ == "__main__":
    folder = 'etf_stock_data'
    os.makedirs('etf_stock_data', exist_ok=True)
    # stocks = get_hs300_stocks()
    # print(f"共获取到 {len(stocks)} 只沪深300成分股")
    # for idx, code in enumerate(stocks):
    #     fetch_and_save_stock_data(code)
    #     if (idx + 1) % 10 == 0:  # 每10次请求后暂停1秒
    #         time.sleep(1)
    # print("所有数据下载完成！")
    # etf_list = [
    #     '159919',
    #     '510500',
    #     '164824',
    #     '513330',
    #     '513520',
    #     '513030'
    # ]
    etf_list = ['510760']
    
    for etf in etf_list:
        fetch_etf_and_save_stock(etf, folder)

    