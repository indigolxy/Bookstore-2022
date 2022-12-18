
#include "UserSystem.h"

User::User(const char *_user_id,const char *_passwd,const char *_user_name,int privilege) : privilege(privilege) {
    strcpy(user_id,_user_id);
    strcpy(passwd,_passwd);
    strcpy(user_name,_user_name);
}

UserSystem::UserSystem() : user_ull("user_other_file.ull","user_main_file.ull"),current_user() {
    file.open("user.main",std::fstream::app | std::fstream::binary);
    if (file.tellp() == 0) { PureAddUser("root","sjtu","root",7); }
    file.close();
    file.clear();
    file.open("user.main",std::fstream::in | std::fstream::out | std::fstream::binary);
}

int UserSystem::FindTheUser(const char *user_id) {
    std::vector<int> find_ans = user_ull.find(user_id);
    if (find_ans.empty()) throw Exception("user doesn't exist");
    return find_ans.front();
}

User UserSystem::ReadUser(int index) {
    User tmp;
    file.seekg(index);
    file.read(reinterpret_cast<char *> (&tmp),sizeof(User));
    return tmp;
}

void UserSystem::Su(const char *user_id,const char *passwd) {
    User target = ReadUser(FindTheUser(user_id));
    if (passwd == nullptr && current_user.privilege <= target.privilege) throw Exception("need password");
    if (passwd != nullptr && strcmp(passwd,target.passwd) != 0) throw Exception("wrong password");

    logged_users.emplace_back(target,-1);
    ++log_in_cnt[user_id];
    current_user = target;
}

void UserSystem::Logout() {
    // 权限不够或无已登录账户(current_user默认privilege = 0)
    if (current_user.privilege < 1) throw Exception("under privilege or no current_user");

    --log_in_cnt[logged_users.back().user.user_id];
    logged_users.pop_back();
    current_user = logged_users.back().user;
}

void UserSystem::PureAddUser(const char *user_id,const char *passwd,const char *user_name,int privilege) noexcept {
    User target(user_id,passwd,user_name,1);
    file.seekp(0,std::ios::end);
    int index = file.tellp();
    file.write(reinterpret_cast<const char *> (&target),sizeof(User));
    log_in_cnt[user_id] = 0;
    user_ull.insert(user_id,index);
}

void UserSystem::Register(const char *user_id, const char *passwd, const char *user_name) {
    std::vector<int> find_ans = user_ull.find(user_id);
    if (!find_ans.empty()) throw Exception("user already exists");
    PureAddUser(user_id,passwd,user_name,1);
}

void UserSystem::UserAdd(const char *user_id, const char *passwd, const int &privilege, const char *user_name) {
    if (current_user.privilege < 3 || privilege >= current_user.privilege) throw Exception("under privilege");
    std::vector<int> find_ans = user_ull.find(user_id);
    if (!find_ans.empty()) throw Exception("user already exists");
    PureAddUser(user_id,passwd,user_name,privilege);
}

void UserSystem::Passwd(const char *user_id, const char *new_passwd, const char *current_passwd) {
    if (current_user.privilege < 1) throw Exception("under privilege");
    int index = FindTheUser(user_id);
    User target = ReadUser(index);
    if (current_passwd == nullptr && current_user.privilege < 7) throw Exception("need password.");
    if (current_passwd != nullptr && strcmp(current_passwd,target.passwd) != 0) throw Exception("wrong password.");

    file.seekp(index + UserMaxSize);
    file.write(reinterpret_cast<const char *> (&new_passwd),UserMaxSize);
}

void UserSystem::Delete(const char *user_id) {
    int index = FindTheUser(user_id);
    if (log_in_cnt[user_id] > 0) throw Exception("user logged in");
    user_ull.remove(user_id,index);
}

void UserSystem::Select(const char *isbn) {
    // todo
}