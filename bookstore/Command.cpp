
#include "Command.h"

std::vector<std::string> SplitString(std::string command) {
    std::vector<std::string> ans;
    for (int i = 0; i < command.length(); ++i) {
        std::string chunk;
        while (command[i] != ' ' && command[i] != '\0') {
            chunk += command[i];
            ++i;
        }
        ans.push_back(chunk);
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

void GetIsbn(char *x, const std::string &chunk) {
    if (chunk.size() <= 6) throw Exception("empty info");
    for (int i = 6;i < chunk.size();++i) {
        x[i - 6] = chunk[i];
    }
}

void GetNameAuthorKeyword(char *x, const std::string &chunk, bool allow_more_keywords) {
    int i = 0;
    while (chunk[i] != '=') {++i;}
    if (chunk[++i] == '\0') throw Exception("empty info");
    int st = ++i;
    for (;i < chunk.size() - 1;++i) {
        if (chunk[i] == '"' || (chunk[i] == '|' && !allow_more_keywords)) throw Exception("invalid input");
        x[i - st] = chunk[i];
    }
}

int processLine(std::string command, UserSystem &user_system, BookSystem &book_system) {
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
        if (split_chunks.size() != 4) throw Exception("invalid input");
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
        if (split_chunks.size() != 5) throw Exception("invalid input");
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
        if (split_chunks.size() == 1) { book_system.ShowAll(user_system); }
        else if (split_chunks[1] == "finance") {
            if (split_chunks.size() == 2) book_system.ShowFinanceAll(user_system);
            else {
                int count = 0;
                try {
                    count = std::stoi(split_chunks[2]);
                } catch (...) {throw Exception("invalid count");}
                book_system.ShowFinance(count, user_system);
            }
        }
        else {
            std::string para = split_chunks[1];
            if (para[1] == 'I') {
                char isbn_tmp[MaxIsbnSize] = {0};
                GetIsbn(isbn_tmp, para);
                book_system.ShowIsbn(isbn_tmp, user_system);
            }
            else if (para[1] == 'n') {
                char name_tmp[MaxBookSize] = {0};;
                GetNameAuthorKeyword(name_tmp, para, false);
                book_system.ShowName(name_tmp,user_system);
            }
            else if (para[1] == 'a') {
                char author_tmp[MaxBookSize] = {0};
                GetNameAuthorKeyword(author_tmp, para, false);
                book_system.ShowAuthor(author_tmp, user_system);
            }
            else if (para[1] == 'k') {
                char key_tmp[MaxBookSize] = {0};
                GetNameAuthorKeyword(key_tmp, para, false);
                book_system.ShowKeyword(key_tmp, user_system);
            }
        }
    }
    else if (first_chunk == "buy") {
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
        char isbn_tmp[MaxIsbnSize] = {0};
        StringToChar(isbn_tmp, split_chunks[1]);
        user_system.Select(isbn_tmp, book_system);
    }
    else if (first_chunk == "modify") {
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
                GetNameAuthorKeyword(name_tmp, para, true);
                modified[1] = true;
            }
            else if (para[1] == 'a') {
                if (modified[2]) throw Exception("repeated modifications");
                GetNameAuthorKeyword(author_tmp, para, true);
                modified[2] = true;
            }
            else if (para[1] == 'k') {
                if (modified[3]) throw Exception("repeated modifications");
                GetNameAuthorKeyword(keyword_tmp, para, true);
                modified[3] = true;
            }
            else if (para[1] == 'p') {
                if (modified[4]) throw Exception("repeated modifications");
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
