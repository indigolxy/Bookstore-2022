//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_USERSYSTEM_H
#define BOOKSTORE_2022_USERSYSTEM_H

#include <cstring>
#include <vector>
#include <unordered_map>
#include "UnrolledLinkList.h"
#include "error.h"

constexpr int UserMaxSize = 35;
constexpr int CommandMaxSize = 1024;

class BookSystem;

class User {
    friend class UserSystem;
    friend class LogInUser;

    char user_id[UserMaxSize] = {0}; // 数字，字母，下划线
    char passwd[UserMaxSize] = {0}; // 数字，字母，下划线
    char user_name[UserMaxSize] = {0}; // 除不可见字符以外 ASCII 字符
    int privilege = 0;

public:
    User() = default;
    explicit User(const char *_user_id,const char *_passwd,const char *_user_name,int privilege);
};

class LogInUser {
    friend class UserSystem;
    friend class BookSystem;
    char user_id[UserMaxSize] = {0};
    int privilege;
    int selected_book_index;
public:
    explicit LogInUser() : privilege(-1), selected_book_index(-1) {}
    explicit LogInUser(const User &user,const int &index = -1) : privilege(user.privilege), selected_book_index(index) {
        strcpy(user_id,user.user_id);
    }
};

class UserSystem {
    friend class BookSystem;
private:
    std::vector<LogInUser> logged_users; //维护登录栈
    std::unordered_map<std::string,int> log_in_cnt;
    // <user_id,times>记录一个用户在登录栈中的登录次数，便于delete操作和判断用户是否登录
    // 每次adduser或register都设置为0
    int current_user_privilege;
    BlockList user_ull; // ("user_other_file","user_main_file") // 块状链表存储已注册用户 user_id-index
    std::fstream file;
    std::fstream log_user_file;

public:
    UserSystem();
    ~UserSystem();

    User ReadUser(int index);

    // * 找到并返回user_id对应的index,若不存在，抛出异常
    int FindTheUser(const char *user_id);

    // * 使登录帐户变为已登录状态，当前帐户变为该帐户,update log_in_cnt
    void Su(const char *user_id, const char *passwd = nullptr);

    void Logout();

    // * 在user.main文件中添加新user，更新log_in_cnt[user_id] = 0,在user.ull中添加新索引
    // * noexcept
    void PureAddUser(const char *user_id, const char *passwd, const char *user_name,int privilege) noexcept;

    // * 类似useradd
    void Register(const char *user_id, const char *passwd, const char *user_name);

    // * 在user.main文件中添加新user，更新log_in_cnt[user_id] = 0,在user.ull中添加新索引
    void UserAdd(const char *user_id, const char *passwd, const int &privilege, const char *user_name);

    // * 只在文件中修改密码
    void Passwd(const char *user_id, const char *new_passwd,const char * current_passwd = nullptr);

    void Delete(const char *user_id);

    void Select (const char *isbn, BookSystem &book_system);

    /*
     * 如果指令合法，将整行指令作为参数传入（show指令不记录）
     * 将command内容和当前操作用户id一起写入log_user_file
     */
    void WriteLogUser(std::string command);

    void ShowLogUser();
};

#endif //BOOKSTORE_2022_USERSYSTEM_H
