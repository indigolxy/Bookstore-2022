//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_USERSYSTEM_H
#define BOOKSTORE_2022_USERSYSTEM_H

#include <string>
#include <vector>

class User;
class UserSystem {
    std::vector<User> logged_user; //维护登录栈
    // todo:块状链表存储已注册用户<UserPair>

public:
    UserSystem();
    ~UserSystem();
    void Su(const std::string &user_id,std::string passwd);
    void Logout();
    void Register(const std::string &user_id,const std::string &passwd,const std::string &username);
    void Passwd(const std::string &user_id,const std::string &new_passwd,std::string current_passwd = "\0");
    void UserAdd(const std::string &user_id,const std::string &passwd,const int &privilege,const std::string &user_name);
    void Delete(const std::string &user_id);
    void Select (const std::string &isbn);
};

class User {
    friend class UserSystem;

    std::string user_id;
    std::string passwd;
    std::string user_name;
    int privilege;
    std::string select_book_isbn; // 选择的图书：无则为空
    int index;

public:
    User();
    User(const std::string &user_id,const std::string &passwd,const std::string &user_name);
    ~User();
};

class UserPair {
    std::string user_id;
    int index;
};


#endif //BOOKSTORE_2022_USERSYSTEM_H
