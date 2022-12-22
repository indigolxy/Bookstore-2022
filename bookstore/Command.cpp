
#include "Command.h"
#include <ctype.h>

std::vector<std::string> SplitString(std::string command) {
    int j = 0;
    while (command[j] == ' ') { ++j; }
    std::vector<std::string> ans;
    for (int i = j; i < command.length(); ++i) {
        std::string chunk;
        while (command[i] != ' ' && i < command.length()) {
            if (!isprint(command[i])) throw Exception("invisible character");
            chunk += command[i];
            ++i;
        }
        ans.push_back(chunk);
        while (i < command.length() - 1 && command[i + 1] == ' ') { ++i; }
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
}

int IsValidPrivilege(const std::string &x) {
    if (x.length() > 1) throw Exception("invalid input");
    if (x[0] == '1' || x[0] == '3' || x[0] == '7') return x[0] - '0';
    throw Exception("invalid input");
}

void GetIsbn(char *x, const std::string &chunk) {
    if (chunk.size() <= 6) throw Exception("empty info");
    if (chunk.size() > 26) throw Exception("too long ISBN");
    for (int i = 6;i < chunk.size();++i) {
        x[i - 6] = chunk[i];
    }
}

void GetName(char *x, const std::string &chunk) {
    if (chunk.size() <= 8) throw Exception("empty info");
    if (chunk.size() > 68) throw Exception("too long book name");
    if (chunk[chunk.length() - 1] != '"') throw Exception("invalid input");

    for (int i = 7; i < chunk.size() - 1 ;++i) {
        if (chunk[i] == '"') throw Exception("invalid input");
        x[i - 7] = chunk[i];
    }
}

void GetAuthor(char *x, const std::string &chunk) {
    if (chunk.size() <= 10) throw Exception("empty info");
    if (chunk.size() > 70) throw Exception("too long book author");
    if (chunk[chunk.length() - 1] != '"') throw Exception("invalid input");

    for (int i = 9; i < chunk.size() - 1 ;++i) {
        if (chunk[i] == '"') throw Exception("invalid input");
        x[i - 9] = chunk[i];
    }
}

void GetKeyword(char *x, const std::string &chunk, bool allow_more_keywords) {
    if (chunk.size() <= 11) throw Exception("empty info");
    if (chunk.size() > 71) throw Exception("too long keyword");
    if (chunk[chunk.length() - 1] != '"') throw Exception("invalid input");

    if (chunk[10] == '|' || chunk[chunk.length() - 2] == '|') throw Exception("empty keyword");

    for (int i = 10; i < chunk.size() - 1 ;++i) {
        if (chunk[i] == '"') throw Exception("invalid input");
        if (chunk[i] == '|' && !allow_more_keywords) throw Exception("more than one keyword");
        if (chunk[i] == '|' && chunk[i + 1] == '|') throw Exception("empty keyword");
        x[i - 10] = chunk[i];
    }
}

int GetQuantityCount(const std::string &x) {
    if (x.length() > 10) throw Exception("too long quantity");
    if (x[0] == '0' && x.length() > 1) throw Exception("invalid 0s in quantity");
    int ans = 0;
    for (int i = 0; i < x.length(); ++i) {
        if (x[i] < '0' || x[i] > '9') throw Exception("not number in quantity");
        ans *= 10;
        ans += x[i] - '0';
    }
    return ans;
}

double GetPriceTotal(const std::string &x) {
    if (x.length() > 13) throw Exception("too long price or total cost");
    if (x[0] == '.' || x[x.length() - 1] == '.') throw Exception("invalid price or total cost");
    if (x[0] == '0' && x.length() > 1) throw Exception("invalid 0s in price or total cost");

    double ans_before = 0, ans_after = 0, div = 1.0;
    bool flag = false;
    for (const char &ch : x) {
        if (ch == '.' && flag) throw Exception("two dots");
        if (ch == '.') {
            flag = true;
            continue;
        }
        if (ch < '0' || ch > '9') throw Exception("not number in price or total cost");
        if (!flag) {
            ans_before *= 10.0;
            ans_before += double(ch - '0');
        }
        else {
            ans_after *= 10.0;
            ans_after += double(ch - '0');
            div *= 10.0;
        }
    }
    if (flag) return (ans_before + ans_after / div);
    else return ans_before;
}

int processLine(std::string command, UserSystem &user_system, BookSystem &book_system) {
    std::vector<std::string> split_chunks = SplitString(command);
    if (split_chunks.empty()) return 1;
    std::string first_chunk =  split_chunks.front();
    if (first_chunk == "quit" || first_chunk == "exit") {
        return 0;
    }
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
                int count = GetQuantityCount(split_chunks[2]);
                book_system.ShowFinance(count, user_system);
            }
            else throw Exception("invalid input");
        }
        else {
            if (split_chunks.size() != 2) throw Exception("invalid input");
            std::string para = split_chunks[1];
            if (para.substr(0,6) == "-ISBN=") {
                char isbn_tmp[MaxIsbnSize] = {0};
                GetIsbn(isbn_tmp, para);
                book_system.ShowIsbn(isbn_tmp, user_system);
            }
            else if (para.substr(0,7) == "-name=\"") {
                char name_tmp[MaxBookSize] = {0};;
                GetName(name_tmp, para);
                book_system.ShowName(name_tmp,user_system);
            }
            else if (para.substr(0,9) == "-author=\"") {
                char author_tmp[MaxBookSize] = {0};
                GetAuthor(author_tmp, para);
                book_system.ShowAuthor(author_tmp, user_system);
            }
            else if (para.substr(0,10) == "-keyword=\"") {
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
        int quantity_tmp = GetQuantityCount(split_chunks[2]);
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
            if (para.substr(0,6) == "-ISBN=") {
                if (modified[0]) throw Exception("repeated modifications");
                GetIsbn(isbn_tmp, para);
                modified[0] = true;
            }
            else if (para.substr(0,7) == "-name=\"") {
                if (modified[1]) throw Exception("repeated modifications");
                GetName(name_tmp, para);
                modified[1] = true;
            }
            else if (para.substr(0,9) == "-author=\"") {
                if (modified[2]) throw Exception("repeated modifications");
                GetAuthor(author_tmp, para);
                modified[2] = true;
            }
            else if (para.substr(0,10) == "-keyword=\"") {
                if (modified[3]) throw Exception("repeated modifications");
                GetKeyword(keyword_tmp, para, true);
                modified[3] = true;
            }
            else if (para.substr(0,7) == "-price=") {
                if (modified[4]) throw Exception("repeated modifications");
                price_tmp = GetPriceTotal(para.substr(7));
                modified[4] = true;
            }
            else throw Exception("invalid input");
        }
        if (!modified[0]) isbn = nullptr;
        if (!modified[1]) name = nullptr;
        if (!modified[2]) author = nullptr;
        if (!modified[3]) key = nullptr;
        book_system.Modify(user_system, isbn, name, author, key, price_tmp);
    }
    else if (first_chunk == "import") {
        if (split_chunks.size() != 3) throw Exception("invalid input");

        int quantity = GetQuantityCount(split_chunks[1]);
        double total_cost = GetPriceTotal(split_chunks[2]);
        book_system.Import(quantity, total_cost, user_system);
    }
    else if (first_chunk == "log") {
        // todo
    }
    else throw Exception("invalid input");
    
    return 1;
}
