//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_USERSYSTEM_H
#define BOOKSTORE_2022_USERSYSTEM_H

#include <cstring>
#include <vector>

class User;
class UserSystem {
    std::vector<User> logged_user; //维护登录栈
    // todo:块状链表存储已注册用户 user_id-index

public:
    UserSystem();
    ~UserSystem();
    void Su(const char* &user_id,const char * &passwd);
    void Logout();
    void Register(const char * &user_id,const char * &passwd,const char * &username);
    void Passwd(const char * &user_id,const char * &new_passwd,char * current_passwd = "\0");
    void UserAdd(const char * &user_id,const char * &passwd,const int &privilege,const char * &user_name);
    void Delete(const char * &user_id);
    void Select (const char * &isbn);
};

class User {
    friend class UserSystem;

    char user_id[35]; // 数字，字母，下划线
    char passwd[35]; // 数字，字母，下划线
    char user_name[35]; // 除不可见字符以外 ASCII 字符
    int privilege;
    char select_book_isbn[25]; // 选择的图书：无则为空
    int index;

public:
    User();
    User(const char * &user_id,const char * &passwd,const char * &user_name);
    ~User();
};

#endif //BOOKSTORE_2022_USERSYSTEM_H
