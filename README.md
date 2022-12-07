# Bookstore-2022开发文档
## 程序功能
1. 维护图书信息
2. 维护账户信息（店主、员工、顾客）
3. 接收并执行指令
4. 维护书店日志（交易信息与操作记录）
## 主体逻辑
1. Command文件统一读入指令并处理并分配给对应的类执行
1. UserSystem类维护账户系统，包括维护登录栈，以及所有已注册账户信息（内嵌的User类维护单个账户信息）（实现select）
2. BookSystem类维护图书系统，内嵌Book类维护每本书的信息。实现检索、修改、购买等功能
3. LogFinance文件维护交易信息日志，记录每一笔交易的收入支出
4. LogUser文件维护操作记录，谁做了什么，按时间顺序
5. ULL实现块状链表，在外存存储所有账户信息与所有图书信息
## 各个类接口与成员说明
### Command.h/Command.cpp
```// 处理字符工具
int NextInt();
std::string NextString();

// 判断指令类型并执行对应函数（若异常，抛出并返回）
// 同时，该函数将每条command内容与操作者记录在LogUser中
// 只处理一行
// main函数中getline循环
void processLine(const std::string &input);

// 基础指令
void CommandQuit();

void CommandExit();

//账户系统指令
void CommandSu(const std::string &user_id,std::string passwd = "\0");

void CommandLogout();

void CommandRegister(const std::string &user_id,const std::string &passwd,const std::string &username);

void CommandPasswd(const std::string &user_id,const std::string &new_passwd,std::string current_passwd = "\0");

void CommandUseradd(const std::string &user_id,const std::string &passwd,const int &privilege,const std::string &user_name);

void CommandDelete(const std::string &user_id);

// select:涉及图书和账户系统
void CommandSelect(const std::string &isbn);

// 图书系统指令
void CommandShow(std::string ) // todo

void CommandBuy(const std::string &isbn,const int &quantity);

void CommandModify( ) // todo

void CommandImport(const int &quantity,const int &total_cost);

// 日志系统指令
void CommandShow(int count = -1); // 缺省时输出全部

void log();
```
### UserSystem.h/UserSystem.cpp
```c++
class UserSystem {
    std::vector<User> logged_user; //维护登录栈
    // todo:块状链表存储已注册用户<UserPair>

public:
    UserSystem();
    ~UserSystem();
    void Su(const std::string &user_id,std::string passwd);
    void Logout();
    void Register(const std::string &user_id,const std::string &passwd,const std::string &username);
    void Passwd(const std::string &user_id,const std::string &new_passwd,std::string current_passwd = "\0");
    void UserAdd(const std::string &user_id,const std::string &passwd,const int &privilege,const std::string &user_name);
    void Delete(const std::string &user_id);
    void Select (const std::string &isbn);
};

class User {
    friend class UserSystem;

    std::string user_id;
    std::string passwd;
    std::string user_name;
    int privilege;
    std::string select_book_isbn; // 选择的图书：无则为空
    int index;

public:
    User();
    User(const std::string &user_id,const std::string &passwd,const std::string &user_name);
    ~User();
};

class UserPair {
    std::string user_id;
    int index;
};

```
### BookSystem.h/BookSystem.cpp
```c++
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
```

### UnrolledLinkList.h/UnrolledLinkList.cpp
```c++
// todo: 需要实现的函数：
void insert(std::string key,int value);
void remove(std::string key,int value);
std::vector<int> find(std::string key); // 找到key对应的所有index（可能会出现在不止一个block中）
```
### LogUser.h/LogUser.cpp
```c++
// 向外存写入完整指令和用户
void WriteLogUser(const std::string &user_id,const std::string &command);

void ShowUserLogAll();
```
### LogFinance.h/LogFinance.cpp
```c++
// 写入外存
void WriteFinanceLogPurchase(const double &amount);
void WriteFinanceLogIncome(const double &amount);

// 从外存读入并求和
void SumFinanceLogPurchaseAll();
void SumFinanceLogPurchase(int count);

void ShowFinanceAll();
void ShowFinance(int count);
```

## 文件存储说明
登录栈存于内存（登录状态的账户和登录顺序及其选择的书）  
已注册的所有用户存于外存（块链）  
所有图书信息存于外存（块链）  
日志-交易信息、操作记录分两个文件存于外存
## 其他补充说明
1. show命令的实现：ULL定义4个变量，分别存isbn,name,author,keyword的index
2. ULL中数据为pair<string,int>按string查找，得到对应的int后到文件对应位置修改或获取book/user的信息
2. 每本书、每个user有一个编号index(int)，按先后顺序表示该组数据(book or user)存在对应文件的第几行
