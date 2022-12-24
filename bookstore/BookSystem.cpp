
#include "BookSystem.h"
#include "Command.h"

BookSystem::BookSystem() : book_isbn_ull("isbn_other.ull","isbn_main.ull"), book_author_ull("author_other.ull","author_main.ull"),
                           book_keyword_ull("keyword_other.ull","keyword_main.ull"), book_name_ull("name_other.ull","name_main.ull"){
    file.open("book.main",std::fstream::app | std::fstream::binary);
    file.close();
    file.clear();
    file.open("book.main",std::fstream::in | std::fstream::out | std::fstream::binary);
    log_finance_file.open("log.finance",std::fstream::app | std::fstream::binary);
    log_finance_file.close();
    log_finance_file.clear();
    log_finance_file.open("log.finance",std::fstream::in | std::fstream::out | std::fstream::binary);
}

BookSystem::~BookSystem() {
    file.close();
    log_finance_file.close();
}

Book BookSystem::ReadBook(int index) {
    file.seekg(index);
    Book tmp;
    file.read(reinterpret_cast<char *> (&tmp),sizeof(Book));
    return tmp;
}

int BookSystem::WriteNewBook(const char *isbn) {
    Book tmp(isbn);
    int index = 0;
    file.seekp(0,std::ios::end);
    index = file.tellp();
    file.write(reinterpret_cast<const char *> (&tmp),sizeof(Book));
    book_isbn_ull.insert(isbn,index);
    return index;
}

void BookSystem::ShowBookSet(const std::vector<int> &ans_index) {
    if (ans_index.empty()) {
        std::cout << '\n';
        return;
    }
    std::set<Book> ans;
    for (const auto &i : ans_index) {
        ans.insert(ReadBook(i));
    }
    for (const auto &x : ans) {
        std::cout << x.isbn << '\t' << x.book_name << '\t' << x.author << '\t' << x.keyword << '\t' << x.price << '\t' << x.quantity << '\n';
    }
}

void BookSystem::ShowAll(const UserSystem &user_system) {
    if (user_system.current_user_privilege < 1) throw Exception("under privilege");

    std::set<Book> ans;
    file.seekg(0);
    while (true) {
        Book tmp;
        file.read(reinterpret_cast<char *> (&tmp), sizeof(Book));
        if (file.eof()) {
            file.clear();
            break;
        }
        ans.insert(tmp);
    }

    if (ans.empty()) {
        std::cout << '\n';
        return;
    }
    for (auto x : ans) {
        std::cout << x.isbn << '\t' << x.book_name << '\t' << x.author << '\t' << x.keyword << '\t' << x.price << '\t' << x.quantity << '\n';
    }
}

void BookSystem::ShowIsbn(const char *isbn, const UserSystem &user_system) {
    if (user_system.current_user_privilege < 1) throw Exception("under privilege");
    std::vector<int> ans_index = book_isbn_ull.find(isbn);
    ShowBookSet(ans_index);
}

void BookSystem::ShowName(const char *book_name, const UserSystem &user_system) {
    if (user_system.current_user_privilege < 1) throw Exception("under privilege");
    std::vector<int> ans_index = book_name_ull.find(book_name);
    ShowBookSet(ans_index);
}

void BookSystem::ShowAuthor(const char *author, const UserSystem &user_system) {
    if (user_system.current_user_privilege < 1) throw Exception("under privilege");
    std::vector<int> ans_index = book_author_ull.find(author);
    ShowBookSet(ans_index);
}

void BookSystem::ShowKeyword(const char *keyword, const UserSystem &user_system) {
    if (user_system.current_user_privilege < 1) throw Exception("under privilege");
    std::vector<int> ans_index = book_keyword_ull.find(keyword);
    ShowBookSet(ans_index);
}

void BookSystem::Buy(const char *isbn, int quantity, const UserSystem &user_system) {
    if (user_system.current_user_privilege < 1) throw Exception("under privilege");
    if (book_isbn_ull.find(isbn).empty()) throw Exception("invalid isbn");
    if (quantity <= 0) throw Exception("unpositive quantity");
    int index = book_isbn_ull.find(isbn).front();
    int quantity_tmp;
    double price_tmp;
    file.seekg(index + MaxIsbnSize + MaxBookSize * 3);
    file.read(reinterpret_cast<char *> (&quantity_tmp),sizeof(int));
    file.read(reinterpret_cast<char *> (&price_tmp),sizeof(double));
    quantity_tmp -= quantity;
    if (quantity_tmp < 0) throw Exception("not enough books");
    file.seekp(index + MaxIsbnSize + MaxBookSize * 3);
    file.write(reinterpret_cast<const char *> (&quantity_tmp), sizeof(int));
    double income = price_tmp * quantity;
    log_finance_file.seekp(0,std::ios::end);
    log_finance_file.write(reinterpret_cast<const char *> (&income), sizeof(double));
    std::cout << income << '\n';
}

std::vector<std::string> BookSystem::GetKeywords(std::string src) {
    std::vector<std::string> ans;
    for (int i = 0; i < src.length(); ++i) {
        std::string chunk;
        while (src[i] != '|' && src[i] != '\0') {
            chunk += src[i];
            ++i;
        }
        for (const auto &x : ans) {
            if (x == chunk) throw Exception("repeated keywords");
        }
        ans.emplace_back(std::move(chunk));
    }
    return ans;
}

void BookSystem::Modify(const UserSystem &user_system, const char *isbn, const char *book_name, const char *author,
                        const char *keyword, const double &price) {
    if (user_system.current_user_privilege < 3) throw Exception("under privilege");
    int index_tmp = user_system.logged_users.back().selected_book_index;
    if (index_tmp == -1) throw Exception("no selected book");

    Book pre_book;
    file.seekg(index_tmp);
    file.read(reinterpret_cast<char *> (&pre_book), sizeof(Book));
    Book modified_book(pre_book);

    if (isbn) {
        if (strcmp(isbn,pre_book.isbn) == 0) throw Exception("same isbn as present");
        if (!book_isbn_ull.find(isbn).empty()) throw Exception("isbn already exists");
        strcpy(modified_book.isbn, isbn);
        book_isbn_ull.remove(pre_book.isbn, index_tmp);
        book_isbn_ull.insert(isbn, index_tmp);
    }
    if (book_name) {
        strcpy(modified_book.book_name, book_name);
        book_name_ull.remove(pre_book.book_name, index_tmp);
        book_name_ull.insert(book_name, index_tmp);
    }
    if (author) {
        strcpy(modified_book.author, author);
        book_author_ull.remove(pre_book.author, index_tmp);
        book_author_ull.insert(author, index_tmp);
    }
    if (keyword) {
        strcpy(modified_book.keyword, keyword);

        std::vector<std::string> pre_keywords = GetKeywords(std::string(pre_book.keyword));
        for (const auto &i : pre_keywords) {
            char key_tmp[MaxBits] = {0};
            StringToChar(key_tmp, i);
            book_keyword_ull.remove(key_tmp,index_tmp);
        }

        std::vector<std::string> modified_keywords = GetKeywords(std::string(keyword));
        for (const auto &i : modified_keywords) {
            char key_tmp[MaxBits] = {0};
            StringToChar(key_tmp, i);
            book_keyword_ull.insert(key_tmp,index_tmp);
        }
    }
    if (price > 0) {
        modified_book.price = price;
    }

    file.seekp(index_tmp);
    file.write(reinterpret_cast<const char *> (&modified_book), sizeof(Book));
}

void BookSystem::Import(const int &quantity, double total_cost, const UserSystem &user_system) {
    if (user_system.current_user_privilege < 3) throw Exception("under privilege");
    int index_tmp = user_system.logged_users.back().selected_book_index;
    if (index_tmp == -1) throw Exception("no selected book");

    if (total_cost <= 0) throw Exception("invalid total cost");

    int pre_quantity = 0;
    file.seekg(index_tmp + MaxIsbnSize + MaxBookSize * 3);
    file.read(reinterpret_cast<char *> (&pre_quantity), sizeof(int));
    pre_quantity += quantity;
    file.seekp(index_tmp + MaxIsbnSize + MaxBookSize * 3);
    file.write(reinterpret_cast<const char *> (&pre_quantity), sizeof(int));

    total_cost = -total_cost;
    log_finance_file.seekp(0,std::ios::end);
    log_finance_file.write(reinterpret_cast<const char *> (&total_cost), sizeof(double));
}

void BookSystem::ShowFinance(int count, const UserSystem &user_system) {
    if (user_system.current_user_privilege < 7)  throw Exception("under privilege");
    if (count == 0) {
        std::cout << std::endl;
        return;
    }
    log_finance_file.seekg(0, std::ios::end);
    int num = log_finance_file.tellg() / sizeof(double);
    if (count > num) throw Exception("count too big");
    log_finance_file.seekg(0 - (count * sizeof(double)), std::ios::end);
    double income = 0.0;
    double cost = 0.0;
    for (int i = 0;i < count;++i) {
        double tmp = 0.0;
        log_finance_file.read(reinterpret_cast<char *> (&tmp), sizeof(double));
        if (tmp < 0) cost += tmp;
        else if (tmp > 0) income += tmp;
    }
    if (cost == 0) std::cout << "+ " << income << " - " << cost << std::endl;
    else std::cout << "+ " << income << " - " << -cost << std::endl;
}

void BookSystem::ShowFinanceAll(const UserSystem &user_system) {
    if (user_system.current_user_privilege < 7)  throw Exception("under privilege");
    double income = 0.0;
    double cost = 0.0;
    log_finance_file.seekg(0);
    while (true) {
        double tmp = 0.0;
        log_finance_file.read(reinterpret_cast<char *> (&tmp), sizeof(double));
        if (log_finance_file.eof()) {
            log_finance_file.clear();
            break;
        }
        if (tmp < 0) cost += tmp;
        else if (tmp > 0) income += tmp;
    }
    if (cost == 0) std::cout << "+ " << income << " - " << cost << std::endl;
    else std::cout << "+ " << income << " - " << -cost << std::endl;
//    if (!bool(log_finance_file)) std::cout << "SHIT" << std::endl;
}