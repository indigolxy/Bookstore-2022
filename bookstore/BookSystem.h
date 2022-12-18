//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_BOOKSYSTEM_H
#define BOOKSTORE_2022_BOOKSYSTEM_H

#include <cstring>
#include "UnrolledLinkList.h"
#include "error.h"

//constexpr int

class Book;
class BookSystem {
private:
    BlockList book_isbn_ull;
    BlockList book_name_ull;
    BlockList book_author_ull;
    BlockList book_keyword_ull;
    std::fstream file;

public:
    void ShowAll();
    void ShowIsbn(const char * &isbn);
    void ShowName(const char * &book_name);
    void ShowAuthor(const char * &author);
    void ShowKeyword(const char * &keyword); // 只能有一个keyword
    void Buy(const char * &isbn,int quantity);
    // 更改后需回到对应的块状链表（eg name_index）中remove再insert更改对应信息
    void ModifyIsbn(const char * &isbn); // 不允许改为原有isbn
    void ModifyName(const char * &book_name);
    void ModifyAuthor(const char * &author);
    void ModifyKeyword(const char * &keyword); // keyword不含重复信息段
    void ModifyPrice(const double &price);
    void Import(const int &quantity,const double &total_cost);
};

class Book {
    friend class BookSystem;

    char isbn[25]; // 不能重复！ // 除不可见字符以外 ASCII 字符
    char book_name[65]; // 除不可见字符和英文双引号以外 ASCII 字符
    char author[65]; // 除不可见字符和英文双引号以外 ASCII 字符
    int keyword_num;
    char keyword[20][65]; // 按输入时顺序存keyword // 除不可见字符和英文双引号以外 ASCII 字符
    int quantity; // 库存数
    double price;
    double total_cost;
    int index;
};

#endif //BOOKSTORE_2022_BOOKSYSTEM_H
