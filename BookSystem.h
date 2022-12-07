//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_BOOKSYSTEM_H
#define BOOKSTORE_2022_BOOKSYSTEM_H

#include <string>

class Book;
class BookSystem {
    // todo:4个块状链表的变量存储isbn_index，name_index，author_index，keyword_index
public:
    void ShowAll();
    void ShowIsbn(const std::string &isbn);
    void ShowName(const std::string &book_name);
    void ShowAuthor(const std::string &author);
    void ShowKeyword(const std::string &keyword); // 只能有一个keyword
    void Buy(const std::string &isbn,int quantity);
    // 更改后需回到对应的块状链表（eg name_index）中remove再insert更改对应信息
    void ModifyIsbn(const std::string &isbn); // 不允许改为原有isbn
    void ModifyName(const std::string &book_name);
    void ModifyAuthor(const std::string &author);
    void ModifyKeyword(const std::string &keyword); // keyword不含重复信息段
    void ModifyPrice(const double &price);
    void Import(const int &quantity,const double &total_cost);
};

class Book {
    friend class BookSystem;

    std::string isbn; // 不能重复！
    std::string book_name;
    std::string author;
    int keyword_num;
    std::string keyword[65]; // 按输入时顺序存keyword
    int quantity; // 库存数
    double price;
    double total_cost;
    int index;
};

class BookPair {
    std::string isbn;
    int index;
};

class BookNamePair {
    std::string book_name;
    int index;
};

class BookAuthorPair {
    std::string author;
    int index;
};

class BookKeywordPair {
    std::string keyword; // 单个keyword
    int index;
};

#endif //BOOKSTORE_2022_BOOKSYSTEM_H
