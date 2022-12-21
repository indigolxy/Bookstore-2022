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
 */
std::vector<std::string> SplitString(std::string command);

/*
 * 判断指令类型并执行对应函数，输入不合法则抛出异常
 * 只处理一行
 * main函数中getline循环
 * 若exit或quit，返回值为0，否则返回1
 */
int processLine(std::string command, UserSystem &user_system, BookSystem &book_system);

/*
 * 将传入的的char*（数组）赋值为y
 * 传入前char数组已初始化{0}
 */
void StringToChar(char *x, const std::string &y);

/*
 * 判断是否是合法的user_id或passwd
 * 若不合法，抛出异常
 */
void IsValidUserid(const std::string &x);

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
 */
void GetIsbn(char *x, const std::string &chunk);

/*
 * chunk格式形如-name="abcd";-keyword="aabbdjj";-author="sdfiwe"
 * 均不能出现“”，keyword中不能出现|（一起判断，抛出异常）
 * 若=号后为空，抛出异常
 */
void GetNameAuthorKeyword(char *x, const std::string &chunk);

#endif //BOOKSTORE_2022_COMMAND_H
