
#include "UserSystem.h"
#include "BookSystem.h"
#include <iomanip>

User::User(const char *_user_id,const char *_passwd,const char *_user_name,int privilege) : privilege(privilege) {
    strcpy(user_id,_user_id);
    strcpy(passwd,_passwd);
    strcpy(user_name,_user_name);
}

UserSystem::UserSystem() : user_ull("user_other.ull","user_main.ull"),current_user_privilege(0) {
    file.open("user.main",std::fstream::app | std::fstream::binary);
    if (file.tellp() == 0) { PureAddUser("root","sjtu","root",7); }
    file.close();
    file.clear();
    file.open("user.main",std::fstream::in | std::fstream::out | std::fstream::binary);
    log_user_file.open("log.user",std::fstream::app | std::fstream::binary);
    log_user_file.close();
    log_user_file.clear();
    log_user_file.open("log.user",std::fstream::in | std::fstream::out | std::fstream::binary);
}

UserSystem::~UserSystem() {
    file.close();
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
    if (passwd == nullptr && current_user_privilege <= target.privilege) throw Exception("need password");
    if (passwd != nullptr && strcmp(passwd,target.passwd) != 0) throw Exception("wrong password");

    logged_users.emplace_back(target,-1);
    ++log_in_cnt[std::string(user_id)];
    current_user_privilege = target.privilege;
}

void UserSystem::Logout() {
    // 权限不够或无已登录账户(current_user默认privilege = 0)
    if (current_user_privilege < 1) throw Exception("under privilege or no current_user");

    --log_in_cnt[std::string(logged_users.back().user_id)];
    logged_users.pop_back();
    if (logged_users.empty()) current_user_privilege = 0;
    else current_user_privilege = logged_users.back().privilege;
}

void UserSystem::PureAddUser(const char *user_id,const char *passwd,const char *user_name,int privilege) noexcept {
    User target(user_id,passwd,user_name,privilege);
    file.seekp(0,std::ios::end);
    int index = file.tellp();
    file.write(reinterpret_cast<const char *> (&target),sizeof(User));
    log_in_cnt[std::string(user_id)] = 0;
    user_ull.insert(user_id,index);
}

void UserSystem::Register(const char *user_id, const char *passwd, const char *user_name) {
    std::vector<int> find_ans = user_ull.find(user_id);
    if (!find_ans.empty()) throw Exception("user already exists");
    PureAddUser(user_id,passwd,user_name,1);
}

void UserSystem::UserAdd(const char *user_id, const char *passwd, const int &privilege, const char *user_name) {
    if (current_user_privilege < 3 || privilege >= current_user_privilege) throw Exception("under privilege");
    if (privilege != 1 && privilege != 3 && privilege != 7) throw Exception("invalid privilege");
    std::vector<int> find_ans = user_ull.find(user_id);
    if (!find_ans.empty()) throw Exception("user already exists");
    PureAddUser(user_id,passwd,user_name,privilege);
}

void UserSystem::Passwd(const char *user_id, const char *new_passwd, const char *current_passwd) {
    if (current_user_privilege < 1) throw Exception("under privilege");
    int index = FindTheUser(user_id);
    User target = ReadUser(index);
    if (current_passwd == nullptr && current_user_privilege < 7) throw Exception("need password.");
    if (current_passwd != nullptr && strcmp(current_passwd,target.passwd) != 0) throw Exception("wrong password.");

    file.seekp(index + UserMaxSize);
    file.write(new_passwd,UserMaxSize);
}

void UserSystem::Delete(const char *user_id) {
    if (current_user_privilege < 7) throw Exception("under privilege");
    int index = FindTheUser(user_id);
    if (log_in_cnt[std::string(user_id)] > 0) throw Exception("user logged in");
    user_ull.remove(user_id, index);
}

void UserSystem::Select(const char *isbn, BookSystem &book_system) {
    if (current_user_privilege < 3) throw Exception("under privilege");
//    if (logged_users.back().selected_book_index != -1) throw Exception("already selected a book");
    std::vector<int> ans = book_system.book_isbn_ull.find(isbn);
    if (ans.empty()) {
        logged_users.back().selected_book_index = book_system.WriteNewBook(isbn);
        return;
    }
    logged_users.back().selected_book_index = ans.front();
}

void UserSystem::WriteLogUser(std::string command) {
    char user_id[UserMaxSize] = {0};
    if (!logged_users.empty()) strcpy(user_id,logged_users.back().user_id);
    char command_[CommandMaxSize] = {0};
    for (int i = 0;i < command.length();++i) {
        command_[i] = command[i];
    }
    log_user_file.seekp(0,std::ios::end);
    log_user_file.write(user_id, UserMaxSize);
    log_user_file.write(command_, CommandMaxSize);
}

void UserSystem::ShowLogUser() {
    log_user_file.seekg(0);
    while (true) {
        std::string user_id;
        char command_[CommandMaxSize] = {0};
        log_user_file.read(reinterpret_cast<char *> (&user_id), UserMaxSize);
        log_user_file.read(command_, CommandMaxSize);

        if (log_user_file.eof()) {
            log_user_file.clear();
            break;
        }

        if (user_id.empty()) std::cout << std::left << std::setw(UserMaxSize) << "visitor:" << " ";
        else std::cout << std::left << std::setw(UserMaxSize) << user_id << ':' << " ";
        std::cout << command_ << std::endl;
    }
}