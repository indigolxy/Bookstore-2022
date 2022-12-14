//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_LOGFINANCE_H
#define BOOKSTORE_2022_LOGFINANCE_H

// 写入外存
void WriteFinanceLogPurchase(const double &amount);
void WriteFinanceLogIncome(const double &amount);

// 从外存读入并求和
void SumFinanceLogPurchaseAll();
void SumFinanceLogPurchase(int count);

void ShowFinanceAll();
void ShowFinance(int count);

#endif //BOOKSTORE_2022_LOGFINANCE_H
