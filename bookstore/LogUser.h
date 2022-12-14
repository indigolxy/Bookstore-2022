//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_LOGUSER_H
#define BOOKSTORE_2022_LOGUSER_H

// 向外存写入完整指令和用户
void WriteLogUser(char user_id[35],const char * &command);

void ShowUserLogAll();

#endif //BOOKSTORE_2022_LOGUSER_H
