//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_COMMAND_H
#define BOOKSTORE_2022_COMMAND_H

#include <exception>
#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include "BookSystem.h"
#include "UserSystem.h"
#include "error.h"

/*
 * 切割一整行的string，返回string类型
 * 以空格为分隔
 * 忽略连续空格和开头结尾的连续空格
 */
std::vector<std::string> SplitString(std::string command);

/*
 * 判断指令类型并执行对应函数，输入不合法则抛出异常
 * 只处理一行
 * main函数中getline循环
 * 若exit或quit，返回值为0，
 * 处理各种不合法情况，调用接下来的小函数
 */
int processLine(std::string command, UserSystem &user_system, BookSystem &book_system);

/*
 * 将传入的的char*（数组）赋值为y
 * 传入前char数组已初始化{0}
 */
void StringToChar(char *x, const std::string &y);

/*
 * 判断是否是合法的user_id或passwd、username
 * 若不合法，抛出异常
 */
void IsValidUseridPasswd(const std::string &x);
void IsValidUsername(const std::string &x);

/*
 * 判断是否为合法的privilege
 * 若不合法，抛出异常
 * 若合法，返回int
 */
int IsValidPrivilege(const std::string &x);

/*
 * 结合substring与StringToChar的功能（调用前char *x数组应被初始化为{0}
 * chunk格式形如-ISBN=aabbxx
 * 返回"aabbxx"（向char*中赋值）
 * 若为-ISBN=，抛出异常empty info
 * 若ISBN长度>20，抛出异常
 * 判断是否为不可见字符
 */
void GetIsbn(char *x, const std::string &chunk);

/*
 * 判断合法性并去除-name=""的格式，给x赋值为需要的内容
 * 注意双引号、信息长度、连续|、参数内容为空等
 */
void GetName(char *x, const std::string &chunk);
void GetAuthor(char *x, const std::string &chunk);
void GetKeyword(char *x, const std::string &chunk, bool allow_more_keywords);

// * 保证quantity是合法正整数（注意前导0）
int GetQuantityCount(const std::string &x);

/*
 * 保证输入是合法的double(>0)
 * 注意前导0、.00、30.、43.42.3等不合法格式
 * 3.00合法
 */
double GetPriceTotal(const std::string &x);

#endif //BOOKSTORE_2022_COMMAND_H
