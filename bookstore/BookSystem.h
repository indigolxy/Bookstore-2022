//
// Created by 李心瑶 on 2022/12/6.
//

#ifndef BOOKSTORE_2022_BOOKSYSTEM_H
#define BOOKSTORE_2022_BOOKSYSTEM_H

#include <cstring>
#include <vector>
#include "UnrolledLinkList.h"
#include "error.h"
#include "UserSystem.h"

constexpr int MaxIsbnSize = 25;
constexpr int MaxBookSize = 65;

class Book;
class BookSystem {
    friend class UserSystem;
private:
    BlockList book_isbn_ull;
    BlockList book_name_ull;
    BlockList book_author_ull;
    BlockList book_keyword_ull;
    std::fstream file;
    std::fstream log_finance_file;

public:
    BookSystem();
    ~BookSystem();

    // * 从file中读一个Book，并返回
    Book ReadBook(int index);

    /*
     * 创建仅拥有 [ISBN] 信息的新图书
     * 写入file末尾
     * 写入book_isbn_ull
     * 返回新book在file中的index
     */
    int WriteNewBook(const char *isbn);

    /*
     * 按isbn升序以一定格式输出一些书的信息
     * [ISBN]\t[BookName]\t[Author]\t[Keyword]\t[Price]\t[库存数量]\n
     * 其中 [Keyword] 中关键词顺序为输入时的顺序。
     * 先读入每本书放入set排序再输出
     */
    void ShowBookSet(const std::vector<int> &ans_index);

    /*
     * 先将所有Book放入内存
     * 用set排序后输出
     */
    void ShowAll(const UserSystem &user_system);

    // * 在ull中搜索得index后，去file中读入并输出
    // ! 按isbn升序输出！调用ShowBookSet
    void ShowIsbn(const char *isbn, const UserSystem &user_system);
    void ShowName(const char *book_name, const UserSystem &user_system);
    void ShowAuthor(const char *author, const UserSystem &user_system);
    // * 只能有一个keyword（需在Command中抛出异常）
    void ShowKeyword(const char *keyword, const UserSystem &user_system);

    /*
     * 只需减少库存，并以浮点数输出购买图书所需的总金额
     * 没有符合条件的图书则操作失败
     * 购买数量为非正整数则操作失败（在本函数处理）
     * 在log_finance_file中记录此次收入
     */
    void Buy(const char *isbn,int quantity, const UserSystem &user_system);

    /*
     * 将一个char数组(keyword)切割为多个单个keyword并以vector<string>返回
     * 若有重复keyword, 抛出异常
     */
    std::vector<std::string> GetKeywords(std::string src);

    /*
     * 以指令中的信息更新选中图书的信息
     * 如未选中图书则操作失败
     * 不允许将 ISBN 改为原有的 ISBN
     * [keyword] 包含重复信息段则操作失败
     * 若未修改某参数，传入nullptr。price传入-1
     * 实现：当作全部modify一遍，在file中更新
     *      在更改了的项目对应的book_xxx_ull中remove再insert更改对应信息(price不需要)
     */
    void Modify(const UserSystem &user_system, const char *isbn, const char *book_name, const char *author,
                const char *keyword, const double &price);

    /*
     * 以指定交易总额购入指定数量的选中图书，增加其库存数。
     * 如未选中图书则操作失败; 购入数量为非正整数则操作失败；交易总额为非正数则操作失败。
     * 在file中更改selected_book的quantity
     * total_cost只在log finance中有用
     * 在log_finance_file中记录此次支出(total_cost)
     */
    void Import(const int &quantity, double total_cost, const UserSystem &user_system);

    /*
     * 从log_finance_file末尾前count * sizeof(double)开始读
     * 按格式输出：+ [收入] - [支出]\n
     * Count 为 0 时输出空行
     * Count 大于历史交易总笔数时操作失败
     */
    void ShowFinance(int count, const UserSystem &user_system);

    // * 返回现有所有交易笔数
    int GetLogCount(const UserSystem &user_system);

    /*
     * 遍历log_finance_file得到每一次记录 ，分收入与支出加和
     * 并按格式输出：+ [收入] - [支出]\n
     * 不存在交易时认为收入支出均为 0.00
     */
    void ShowFinanceAll(const UserSystem &user_system);
};

class Book {
    friend class BookSystem;

    char isbn[MaxIsbnSize] = {0};
    char book_name[MaxBookSize] = {0};
    char author[MaxBookSize] = {0};
    char keyword[MaxBookSize] = {0}; // 按输入时格式存keyword
    int quantity = 0; // 库存数
    double price = 0.0;

public:
    Book() = default;
    explicit Book(const char *_isbn) {
        strcpy(isbn,_isbn);
    }
    Book(const Book &x) = default;
    bool operator<(const Book &obj) const {
//        return (strcmp(isbn, obj.isbn) < 0);
        for (int i = 0; i < MaxIsbnSize; ++i) {
            if (isbn[i] != obj.isbn[i])
                return isbn[i] < obj.isbn[i];
        }
        return false;
    }

};

#endif //BOOKSTORE_2022_BOOKSYSTEM_H
