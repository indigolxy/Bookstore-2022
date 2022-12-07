//
// Created by 李心瑶 on 2022/12/7.
//

#ifndef BOOKSTORE_2022_UNROLLEDLINKLIST_H
#define BOOKSTORE_2022_UNROLLEDLINKLIST_H

#include <string>
#include <vector>

// todo: 需要实现的函数：
void insert(std::string key,int value);
void remove(std::string key,int value);
std::vector<int> find(std::string key); // 找到key对应的所有index（可能会出现在不止一个block中）

#endif //BOOKSTORE_2022_UNROLLEDLINKLIST_H
