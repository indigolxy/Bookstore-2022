//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_COMMAND_H
#define BOOKSTORE_2022_COMMAND_H

// 处理字符工具
int NextInt();
char* NextString();

// 判断指令类型并执行对应函数（若异常，抛出并返回）
// 同时，该函数将每条command内容与操作者记录在LogUser中
// 只处理一行
// main函数中getline循环
void processLine(const char * &input);

// 基础指令
void CommandQuit();

void CommandExit();

//账户系统指令
void CommandSu(const char * &user_id,char *passwd = nullptr);

void CommandLogout();

void CommandRegister(const char * &user_id,const char * &passwd,const char * &username);

void CommandPasswd(const char * &user_id,const char * &passwd,char *current_passwd = nullptr);

void CommandUseradd(const char * &user_id,const char * &passwd,const int &privilege,const char * &username);

void CommandDelete(const char * &user_id);

// select:涉及图书和账户系统
void CommandSelect(const char * &isbn);

// 图书系统指令
void CommandShow( ) // todo

void CommandBuy(const char * &user_id,const int &quantity);

void CommandModify( ) // todo

void CommandImport(const int &quantity,const int &total_cost);

// 日志系统指令
void CommandShow(int count = -1); // 缺省时输出全部

void log();

#endif //BOOKSTORE_2022_COMMAND_H
