
#include "Command.h"
#include<ctype.h>

std::vector<std::string> SplitString(std::string command) {
    int j = 0;
    while (command[j] == ' ') { ++j; }
    std::vector<std::string> ans;
    for (int i = j; i < command.length(); ++i) {
        std::string chunk;
        while (command[i] != ' ' && i < command.length()) {
            chunk += command[i];
            ++i;
        }
        ans.push_back(chunk);
        while (command[i + 1] == ' ') { ++i; }
    }
    return ans;
}

void StringToChar(char *x,const std::string &y) {
    for (int i = 0;i < y.length();++i) {
        x[i] = y[i];
    }
}

void IsValidUseridPasswd(const std::string &x) {
    if (x.length() > 30) throw Exception("too long userid or password");
    for (int i = 0;i < x.length();++i) {
        if ((x[i] >= 'A' && x[i] <= 'Z') || (x[i] >= 'a' && x[i] <= 'z') || (x[i] >= '0' && x[i] <= '9') || x[i] == '_') continue;
        else throw Exception("invalid input");
    }
}

void IsValidUsername(const std::string &x) {
    if (x.length() > 30) throw Exception("too long username");
    for (int i = 0;i < x.length();++i) {
        if (!std::isprint(x[i])) throw Exception("is not print character");
    }
}

int IsValidPrivilege(const std::string &x) {
    if (x.length() > 1) throw Exception("invalid input");
    if (x[0] == '1' || x[0] == '3' || x[0] == '7') return x[0] - '0';
    throw Exception("invalid input");
}

void GetIsbn(char *x, const std::string &chunk) {
    if (chunk.size() <= 6) throw Exception("empty info");
    if (chunk.size() > 26) throw Exception("too long ISBN");
    if (chunk.substr(0,6) != "-ISBN=") throw Exception("invalid input");
    for (int i = 6;i < chunk.size();++i) {
        if (!std::isprint(chunk[i])) throw Exception("is not print character");
        x[i - 6] = chunk[i];
    }
}

void GetName(char *x, const std::string &chunk) {
    if (chunk.size() <= 8) throw Exception("empty info");
    if (chunk.size() > 68) throw Exception("too long book name");
    if (chunk.substr(0,7) != "-name=\"") throw Exception("invalid input");
    if (chunk[chunk.length() - 1] != '"') throw Exception("invalid input");

    for (int i = 7; i < chunk.size() - 1 ;++i) {
        if (!std::isprint(chunk[i])) throw Exception("book name is not print character");
        if (chunk[i] == '"') throw Exception("invalid input");
        x[i - 7] = chunk[i];
    }
}

void GetAuthor(char *x, const std::string &chunk) {
    if (chunk.size() <= 10) throw Exception("empty info");
    if (chunk.size() > 70) throw Exception("too long book author");
    if (chunk.substr(0,9) != "-author=\"") throw Exception("invalid input");
    if (chunk[chunk.length() - 1] != '"') throw Exception("invalid input");

    for (int i = 9; i < chunk.size() - 1 ;++i) {
        if (!std::isprint(chunk[i])) throw Exception("author is not print character");
        if (chunk[i] == '"') throw Exception("invalid input");
        x[i - 9] = chunk[i];
    }
}

void GetKeyword(char *x, const std::string &chunk, bool allow_more_keywords) {
    if (chunk.size() <= 11) throw Exception("empty info");
    if (chunk.size() > 71) throw Exception("too long keyword");
    if (chunk.substr(0,10) != "-keyword=\"") throw Exception("invalid input");
    if (chunk[chunk.length() - 1] != '"') throw Exception("invalid input");

    for (int i = 10; i < chunk.size() - 1 ;++i) {
        if (!std::isprint(chunk[i])) throw Exception("keyword is not print character");
        if (chunk[i] == '"') throw Exception("invalid input");
        if (chunk[i] == '|' && !allow_more_keywords) throw Exception("more than one keyword");
        if (chunk[i] == '|' && chunk[i + 1] == '|') throw Exception("empty keyword");
        x[i - 10] = chunk[i];
    }
}

int processLine(std::string command, UserSystem &user_system, BookSystem &book_system) {
    std::vector<std::string> split_chunks = SplitString(command);
    if (split_chunks.empty()) return 1;
    std::string first_chunk =  split_chunks.front();
    if (first_chunk == "quit" || first_chunk == "exit") return 0;
    char user_id[UserMaxSize] = {0};
    char user_passwd[UserMaxSize] = {0};
    char user_name[UserMaxSize] = {0};
    char user_new_passwd[UserMaxSize] = {0};

    if (first_chunk == "su") {
        if (split_chunks.size() == 2) {
            IsValidUseridPasswd(split_chunks[1]);
            StringToChar(user_id,split_chunks[1]);
            user_system.Su(user_id);
        }
        else if (split_chunks.size() == 3) {
            IsValidUseridPasswd(split_chunks[1]);
            IsValidUseridPasswd(split_chunks[2]);

            StringToChar(user_id,split_chunks[1]);
            StringToChar(user_passwd,split_chunks[2]);

            user_system.Su(user_id, user_passwd);
        }
        else throw Exception("invalid input");
    }
    else if (first_chunk == "logout") {
        if (split_chunks.size() != 1) throw Exception("invalid input");
        user_system.Logout();
    }
    else if (first_chunk == "register") {
        if (split_chunks.size() != 4) throw Exception("invalid input");

        IsValidUseridPasswd(split_chunks[1]);
        IsValidUseridPasswd(split_chunks[2]);
        IsValidUsername(split_chunks[3]);

        StringToChar(user_id, split_chunks[1]);
        StringToChar(user_passwd, split_chunks[2]);
        StringToChar(user_name, split_chunks[3]);

        user_system.Register(user_id, user_passwd, user_name);
    }
    else if (first_chunk == "passwd") {
        if (split_chunks.size() == 3) {
            IsValidUseridPasswd(split_chunks[1]);
            IsValidUseridPasswd(split_chunks[2]);

            StringToChar(user_id, split_chunks[1]);
            StringToChar(user_new_passwd, split_chunks[2]);

            user_system.Passwd(user_id, user_new_passwd);
        }
        else if (split_chunks.size() == 4) {
            IsValidUseridPasswd(split_chunks[1]);
            IsValidUseridPasswd(split_chunks[2]);
            IsValidUseridPasswd(split_chunks[3]);

            StringToChar(user_id, split_chunks[1]);
            StringToChar(user_new_passwd, split_chunks[3]);
            StringToChar(user_passwd, split_chunks[2]);
            user_system.Passwd(user_id, user_new_passwd, user_passwd);
        }
        else throw Exception("invalid input");
    }
    else if (first_chunk == "useradd") {
        if (split_chunks.size() != 5) throw Exception("invalid input");
        IsValidUseridPasswd(split_chunks[1]);
        IsValidUseridPasswd(split_chunks[2]);
        int privilege = IsValidPrivilege(split_chunks[3]);
        IsValidUsername(split_chunks[4]);

        StringToChar(user_id, split_chunks[1]);
        StringToChar(user_passwd, split_chunks[2]);
        StringToChar(user_name, split_chunks[4]);

        user_system.UserAdd(user_id, user_passwd, privilege, user_name);
    }
    else if (first_chunk == "delete") {
        if (split_chunks.size() != 2) throw Exception("invalid input");
        IsValidUseridPasswd(split_chunks[1]);
        StringToChar(user_id, split_chunks[1]);
        user_system.Delete(user_id);
    }
    else if (first_chunk == "show") {
        if (split_chunks.size() == 1) { book_system.ShowAll(user_system); }
        else if (split_chunks[1] == "finance") {
            if (split_chunks.size() == 2) book_system.ShowFinanceAll(user_system);
            else if (split_chunks.size() == 3){
                if (split_chunks[2].size() > 10) throw Exception("too long count");
                int count = 0;
                try {
                    count = std::stoi(split_chunks[2]);
                } catch (...) {throw Exception("invalid count");}
                book_system.ShowFinance(count, user_system);
            }
            else throw Exception("invalid input");
        }
        else {
            if (split_chunks.size() != 2) throw Exception("invalid input");
            std::string para = split_chunks[1];
            if (para[1] == 'I') {
                char isbn_tmp[MaxIsbnSize] = {0};
                GetIsbn(isbn_tmp, para);
                book_system.ShowIsbn(isbn_tmp, user_system);
            }
            else if (para[1] == 'n') {
                char name_tmp[MaxBookSize] = {0};;
                GetName(name_tmp, para);
                book_system.ShowName(name_tmp,user_system);
            }
            else if (para[1] == 'a') {
                char author_tmp[MaxBookSize] = {0};
                GetAuthor(author_tmp, para);
                book_system.ShowAuthor(author_tmp, user_system);
            }
            else if (para[1] == 'k') {
                char key_tmp[MaxBookSize] = {0};
                GetKeyword(key_tmp, para, false);
                book_system.ShowKeyword(key_tmp, user_system);
            }
            else throw Exception("invalid input");
        }
    }
    else if (first_chunk == "buy") {
        if (split_chunks.size() != 3) throw Exception("invalid input");
        if (split_chunks[1].size() > 20) throw Exception("too long isbn");
        if (split_chunks[2].size() > 10) throw Exception("too long quantity");

        char isbn_tmp[MaxIsbnSize] = {0};
        StringToChar(isbn_tmp, split_chunks[1]);
        int quantity_tmp = 0;
        try {
            quantity_tmp = std::stoi(split_chunks[2]);
        }
        catch (...) {
            throw Exception("invalid quantity");
        }
        book_system.Buy(isbn_tmp, quantity_tmp, user_system);
    }
    else if (first_chunk == "select") {
        if (split_chunks.size() != 2) throw Exception("invalid input");
        if (split_chunks[1].size() > 20) throw Exception("too long isbn");

        char isbn_tmp[MaxIsbnSize] = {0};
        StringToChar(isbn_tmp, split_chunks[1]);
        user_system.Select(isbn_tmp, book_system);
    }
    else if (first_chunk == "modify") {
        if (split_chunks.size() == 1) throw Exception("invalid input");
        // * 有重复附加参数为非法指令； 附加参数内容为空则操作失败；
        char isbn_tmp[MaxIsbnSize] = {0};
        char *isbn = isbn_tmp;
        char name_tmp[MaxBookSize] = {0};
        char *name = name_tmp;
        char author_tmp[MaxBookSize] = {0};
        char *author = author_tmp;
        char keyword_tmp[MaxBookSize] = {0};
        char *key = keyword_tmp;
        double price_tmp = -1;
        bool modified[5] = {false};

        for (int i = 1;i < split_chunks.size();++i) {
            std::string para = split_chunks[i];
            if (para[1] == 'I') {
                if (modified[0]) throw Exception("repeated modifications");
                GetIsbn(isbn_tmp, para);
                modified[0] = true;
            }
            else if (para[1] == 'n') {
                if (modified[1]) throw Exception("repeated modifications");
                GetName(name_tmp, para);
                modified[1] = true;
            }
            else if (para[1] == 'a') {
                if (modified[2]) throw Exception("repeated modifications");
                GetAuthor(author_tmp, para);
                modified[2] = true;
            }
            else if (para[1] == 'k') {
                if (modified[3]) throw Exception("repeated modifications");
                GetKeyword(keyword_tmp, para, true);
                modified[3] = true;
            }
            else if (para[1] == 'p') {
                if (modified[4]) throw Exception("repeated modifications");
                if (para.substr(7).size() > 13) throw Exception("too long price");

                try {
                    price_tmp = std::stod(para.substr(7));
                }
                catch (...) { throw Exception("invalid price"); }
                modified[4] = true;
            }
        }
        if (!modified[0]) isbn = nullptr;
        if (!modified[1]) name = nullptr;
        if (!modified[2]) author = nullptr;
        if (!modified[3]) key = nullptr;
        book_system.Modify(user_system, isbn, name, author, key, price_tmp);
    }
    else if (first_chunk == "import") {
        if (split_chunks.size() != 3) throw Exception("invalid input");
        if (split_chunks[1].size() > 10) throw Exception("too long quantity");
        if (split_chunks[2].size() > 13) throw Exception("too long total cost");

        int quantity = 0;
        double total_cost = 0;
        try {
            quantity = std::stoi(split_chunks[1]);
            total_cost = std::stod(split_chunks[2]);
        }
        catch (...) { throw Exception("invalid input"); }
        book_system.Import(quantity, total_cost, user_system);
    }
    else if (first_chunk == "log") {
        // todo
    }
    else throw Exception("invalid input");
    
    return 1;
}
