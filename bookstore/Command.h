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

// 处理字符工具
int NextInt();
char* NextString();

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
int processLine(std::string command, UserSystem &user_system);

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

#endif //BOOKSTORE_2022_COMMAND_H
