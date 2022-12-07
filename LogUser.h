//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_LOGUSER_H
#define BOOKSTORE_2022_LOGUSER_H

#include <string>

// 向外存写入完整指令和用户
void WriteLogUser(const std::string &user_id,const std::string &command);

void ShowUserLogAll();

#endif //BOOKSTORE_2022_LOGUSER_H
