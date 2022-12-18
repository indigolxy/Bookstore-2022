
#include "Command.h"

std::vector<std::string> SplitString(std::string command) {
    std::vector<std::string> ans;
    for (int i = 0; i < command.length(); ++i) {
        std::string chunk;
        while (command[i] != ' ' && command[i] != '\0') {
            chunk += command[i];
            ++i;
        }
        ans.emplace_back(std::move(chunk));
    }
    return ans;
}

void StringToChar(char *x,const std::string &y) {
    for (int i = 0;i < y.length();++i) {
        x[i] = y[i];
    }
}

void IsValidUserid(const std::string &x) {
    if (x.length() > 30) throw Exception("invalid input");
    for (int i = 0;i < x.length();++i) {
        if ((x[i] >= 'A' && x[i] <= 'Z') || (x[i] >= 'a' && x[i] <= 'z') || (x[i] >= '0' && x[i] <= '9') || x[i] == '_') continue;
        else throw Exception("invalid input");
    }
}

int IsValidPrivilege(const std::string &x) {
    if (x.length() > 1) throw Exception("invalid input");
    if (x[0] == '1' || x[0] == '3' || x[0] == '7') return x[0] - '0';
    throw Exception("invalid input");
}

int processLine(std::string command, UserSystem &user_system) {
    std::vector<std::string> split_chunks = SplitString(command);
    std::string first_chunk =  split_chunks.front();
    if (first_chunk == "quit" || first_chunk == "exit") return 0;
    char user_id[UserMaxSize] = {0};
    char user_passwd[UserMaxSize] = {0};
    char user_name[UserMaxSize] = {0};
    char user_new_passwd[UserMaxSize] = {0};

    if (first_chunk == "su") {
        if (split_chunks.size() == 2) {
            StringToChar(user_id,split_chunks[1]);
            user_system.Su(user_id);
        }
        else if (split_chunks.size() == 3) {
            StringToChar(user_id,split_chunks[1]);
            StringToChar(user_passwd,split_chunks[2]);
            user_system.Su(user_id, user_passwd);
        }
    }
    else if (first_chunk == "logout") {
        user_system.Logout();
    }
    else if (first_chunk == "register") {
        IsValidUserid(split_chunks[1]);
        IsValidUserid(split_chunks[2]);
        StringToChar(user_id, split_chunks[1]);
        StringToChar(user_passwd, split_chunks[2]);
        StringToChar(user_name, split_chunks[3]);
        user_system.Register(user_id, user_passwd, user_name);
    }
    else if (first_chunk == "passwd") {
        if (split_chunks.size() == 3) {
            IsValidUserid(split_chunks[2]);
            StringToChar(user_id, split_chunks[1]);
            StringToChar(user_new_passwd, split_chunks[2]);
            user_system.Passwd(user_id, user_new_passwd);
        }
        else {
            IsValidUserid(split_chunks[3]);
            StringToChar(user_id, split_chunks[1]);
            StringToChar(user_new_passwd, split_chunks[3]);
            StringToChar(user_passwd, split_chunks[2]);
            user_system.Passwd(user_id, user_new_passwd, user_passwd);
        }
    }
    else if (first_chunk == "useradd") {
        IsValidUserid(split_chunks[1]);
        IsValidUserid(split_chunks[2]);
        int privilege = IsValidPrivilege(split_chunks[3]);
        StringToChar(user_id, split_chunks[1]);
        StringToChar(user_passwd, split_chunks[2]);
        StringToChar(user_name, split_chunks[4]);
        user_system.UserAdd(user_id, user_passwd, privilege, user_name);
    }
    else if (first_chunk == "delete") {
        StringToChar(user_id, split_chunks[1]);
        user_system.Delete(user_id);
    }
    else if (first_chunk == "show") {
        if (split_chunks[1] == "finance") {
            // todo
        }
        else {
            // todo
        }
    }
    else if (first_chunk == "buy") {
        // todo
    }
    else if (first_chunk == "select") {
        // todo
    }
    else if (first_chunk == "modify") {
        // todo
    }
    else if (first_chunk == "import") {
        // todo
    }
    else if (first_chunk == "log") {
        // todo
    }
    
    return 1;
}
