
#include "Command.h"

std::vector<std::string> SplitString(std::string command) {
    std::vector<std::string> ans;
    for (int i = 0;i < command.length();++i) {
        std::string chunk;
        while (command[i] != ' ' && command[i] != '\0') {
            chunk += command[i];
            ++i;
        }
        ans.emplace_back(std::move(chunk));
    }
    return ans;
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
    if (first_chunk == "su") {
        if (split_chunks.size() == 2) {
            user_system.Su(split_chunks[1].data());
        }
        else if (split_chunks.size() == 3) {
            user_system.Su(split_chunks[1].data(), split_chunks[2].data());
        }
    }
    else if (first_chunk == "logout") {
        user_system.Logout();
    }
    else if (first_chunk == "register") {
        IsValidUserid(split_chunks[1]);
        IsValidUserid(split_chunks[2]);
        user_system.Register(split_chunks[1].data(), split_chunks[2].data(), split_chunks[3].data());
    }
    else if (first_chunk == "passwd") {
        if (split_chunks.size() == 3) {
            IsValidUserid(split_chunks[2]);
            user_system.Passwd(split_chunks[1].data(), split_chunks[2].data());
        }
        else {
            IsValidUserid(split_chunks[3]);
            user_system.Passwd(split_chunks[1].data(), split_chunks[3].data(), split_chunks[2].data());
        }
    }
    else if (first_chunk == "useradd") {
        IsValidUserid(split_chunks[1]);
        IsValidUserid(split_chunks[2]);
        int privilege = IsValidPrivilege(split_chunks[3]);
        user_system.UserAdd(split_chunks[1].data(), split_chunks[2].data(), privilege, split_chunks[4].data());
    }
    else if (first_chunk == "delete") {
        user_system.Delete(split_chunks[1].data());
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
