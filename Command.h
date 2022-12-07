//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_COMMAND_H
#define BOOKSTORE_2022_COMMAND_H

#include <string>

// 处理字符工具
int NextInt();
std::string NextString();

// 判断指令类型并执行对应函数（若异常，抛出并返回）
// 同时，该函数将每条command内容与操作者记录在LogUser中
// 只处理一行
// main函数中getline循环
void processLine(const std::string &input);

// 基础指令
void CommandQuit();

void CommandExit();

//账户系统指令
void CommandSu(const std::string &user_id,std::string passwd = "\0");

void CommandLogout();

void CommandRegister(const std::string &user_id,const std::string &passwd,const std::string &username);

void CommandPasswd(const std::string &user_id,const std::string &new_passwd,std::string current_passwd = "\0");

void CommandUseradd(const std::string &user_id,const std::string &passwd,const int &privilege,const std::string &user_name);

void CommandDelete(const std::string &user_id);

// select:涉及图书和账户系统
void CommandSelect(const std::string &isbn);

// 图书系统指令
void CommandShow(std::string ) // todo

void CommandBuy(const std::string &isbn,const int &quantity);

void CommandModify( ) // todo

void CommandImport(const int &quantity,const int &total_cost);

// 日志系统指令
void CommandShow(int count = -1); // 缺省时输出全部

void log();

#endif //BOOKSTORE_2022_COMMAND_H
