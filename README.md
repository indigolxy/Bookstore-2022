# Bookstore-2022开发文档
## 程序功能
1. 维护图书信息
2. 维护账户信息（店主、员工、顾客）
3. 接收并执行指令
4. 维护书店日志（交易信息与操作记录）
## 主体逻辑
1. Command文件统一读入指令并处理并分配给对应的类执行
   同时解决不合法输入并抛出异常
1. UserSystem类维护账户系统，包括维护登录栈，以及所有已注册账户信息（内嵌的User类维护单个账户信息）（实现select）
2. BookSystem类维护图书系统，内嵌Book类维护每本书的信息。实现检索、修改、购买等功能
5. ULL实现块状链表，在外存存储所有账户信息与所有图书信息
6. log实现：log finance在book system中实现，log user在user system中实现
## 各个类接口与成员说明
### Command.h/Command.cpp
```c++
/*
 * 切割一整行的string，返回string类型
 * 以空格为分隔
 * 忽略连续空格和开头结尾的连续空格
 */
std::vector<std::string> SplitString(std::string command);

/*
 * 判断指令类型并执行对应函数，输入不合法则抛出异常
 * 只处理一行
 * main函数中getline循环
 * 若exit或quit，返回值为0，
 * 处理各种不合法情况，调用接下来的小函数
 */
int processLine(std::string command, UserSystem &user_system, BookSystem &book_system);

/*
 * 将传入的的char*（数组）赋值为y
 * 传入前char数组已初始化{0}
 */
void StringToChar(char *x, const std::string &y);

/*
 * 判断是否是合法的user_id或passwd、username
 * 若不合法，抛出异常
 */
void IsValidUseridPasswd(const std::string &x);
void IsValidUsername(const std::string &x);

/*
 * 判断是否为合法的privilege
 * 若不合法，抛出异常
 * 若合法，返回int
 */
int IsValidPrivilege(const std::string &x);

/*
 * 结合substring与StringToChar的功能（调用前char *x数组应被初始化为{0}
 * chunk格式形如-ISBN=aabbxx
 * 返回"aabbxx"（向char*中赋值）
 * 若为-ISBN=，抛出异常empty info
 * 若ISBN长度>20，抛出异常
 * 判断是否为不可见字符
 */
void GetIsbn(char *x, const std::string &chunk);

/*
 * 判断合法性并去除-name=""的格式，给x赋值为需要的内容
 * 注意双引号、信息长度、连续|、参数内容为空等
 */
void GetName(char *x, const std::string &chunk);
void GetAuthor(char *x, const std::string &chunk);
void GetKeyword(char *x, const std::string &chunk, bool allow_more_keywords);

// * 保证quantity是合法正整数（注意前导0）
int GetQuantityCount(const std::string &x);

/*
 * 保证输入是合法的double(>0)
 * 注意前导0、.00、30.、43.42.3等不合法格式
 * 3.00合法
 */
double GetPriceTotal(const std::string &x);
```

### UserSystem.h/UserSystem.cpp
```c++
class User {
    friend class UserSystem;
    friend class LogInUser;

    char user_id[UserMaxSize] = {0}; // 数字，字母，下划线
    char passwd[UserMaxSize] = {0}; // 数字，字母，下划线
    char user_name[UserMaxSize] = {0}; // 除不可见字符以外 ASCII 字符
    int privilege = 0;

public:
    User() = default;
    explicit User(const char *_user_id,const char *_passwd,const char *_user_name,int privilege);
};

class LogInUser {
    friend class UserSystem;
    friend class BookSystem;
    char user_id[UserMaxSize] = {0};
    int privilege;
    int selected_book_index;
public:
    explicit LogInUser() : privilege(-1), selected_book_index(-1) {}
    explicit LogInUser(const User &user,const int &index = -1) : privilege(user.privilege), selected_book_index(index) {
        strcpy(user_id,user.user_id);
    }
};

class UserSystem {
    friend class BookSystem;
private:
    std::vector<LogInUser> logged_users; //维护登录栈
    std::unordered_map<std::string,int> log_in_cnt;
    // <user_id,times>记录一个用户在登录栈中的登录次数，便于delete操作和判断用户是否登录
    // 每次adduser或register都设置为0
    int current_user_privilege;
    BlockList user_ull; // ("user_other_file","user_main_file") // 块状链表存储已注册用户 user_id-index
    std::fstream file;
    std::fstream log_user_file;

public:
    UserSystem();
    ~UserSystem();

    User ReadUser(int index);

    // * 找到并返回user_id对应的index,若不存在，抛出异常
    int FindTheUser(const char *user_id);

    // * 使登录帐户变为已登录状态，当前帐户变为该帐户,update log_in_cnt
    void Su(const char *user_id, const char *passwd = nullptr);

    void Logout();

    // * 在user.main文件中添加新user，更新log_in_cnt[user_id] = 0,在user.ull中添加新索引
    // * noexcept
    void PureAddUser(const char *user_id, const char *passwd, const char *user_name,int privilege) noexcept;

    // * 类似useradd
    void Register(const char *user_id, const char *passwd, const char *user_name);

    // * 在user.main文件中添加新user，更新log_in_cnt[user_id] = 0,在user.ull中添加新索引
    void UserAdd(const char *user_id, const char *passwd, const int &privilege, const char *user_name);

    // * 只在文件中修改密码
    void Passwd(const char *user_id, const char *new_passwd,const char * current_passwd = nullptr);

    void Delete(const char *user_id);

    void Select (const char *isbn, BookSystem &book_system);

    /*
     * 如果指令合法，将整行指令作为参数传入（show指令不记录）
     * 将command内容和当前操作用户id一起写入log_user_file
     */
    void WriteLogUser(std::string command);

    void ShowLogUser();
};
```

### BookSystem.h/BookSystem.cpp
```c++
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

```

### UnrolledLinkList.h/UnrolledLinkList.cpp
```c++
class _pair {
public:
    char index[MaxBits] = {'\0'};
    int value;
    _pair(const char * _index,int value) : value(value){
        strcpy(index,_index);
    }
    _pair() : value(0) {}

    bool operator==(const _pair &obj);

    bool operator<(const _pair &obj);

    bool operator>(const _pair &obj) { return !(*this < obj || *this == obj); }

    bool operator<=(const _pair &obj) { return *this < obj || *this == obj; }

    bool operator>=(const _pair &obj) { return !(*this < obj); }
};

class node {
    friend class BlockList;
    _pair st;
    _pair ed;
    int size; // !!1-base!!
    long prev;
    long next;
    _pair data[MAXSIZE];

public:
    node() : prev(-1),next(-1),size(1) {};
    node(_pair st,_pair ed,int size,long prev,long next) : st(st),ed(ed),size(size),prev(prev),next(next) {}
};

#pragma pack(pop)

class BlockList {
    std::string other_file;
//    friend int main();
    std::fstream file; // "other.txt" "file.txt"
    bool flag_start = false;
    long tail = 0;

public:

    /*void printAll() {
        long p = 0;
        while (p != -1) {
            node nd = ReadNode(p);
            p = nd.next;
            std::cout << "[" << nd.st.index << "," << nd.st.value << "] ";
            std::cout << "{" << nd.ed.index << "," << nd.ed.value << "} ";

            for (int i = 0; i < nd.size; ++i) {
                std::cout << "(" << nd.data[i].index << "," << nd.data[i].value << ") ";
            }
            std::cout << std::endl;
        }
    }*/

    BlockList(std::string other_file,std::string main_file);

    ~BlockList();

    /*
     * 从file的pos位置读入一个完整node
     * 函数执行结束后读指针走到data的最后
     */

    node ReadNode(const long &pos);

    /*
     * 向file的pos位置中写入一个完整的node
     * 写完后写指针移动至data[MAXSIZE - 1]的最后(empty_pair也要写)
     * 若在文件最后写入，pos == -1
     * 否则pos为要写入的块的位置
     */

    void WriteNode(const node &obj,const long &pos);

    /*
     * 找到target应当属于的block（无论target是否存在）
     * 若target属于一个block的[st,ed]，返回该block的起始位置
     * 若target位于两个block的ed和st之间，返回后一个block的起始位置
     * 若target>最大的ed，返回-1
     */

    long FindTheBlock(const _pair &target);

    /*
     * 找到第一个index_pair应当出现的block（无论index是否存在）
     * 若index属于一个block的[st,ed]，返回该block的起始位置
     * 若index位于两个block的ed和st之间，返回后一个block的起始位置
     * 若index>最大的ed，返回-1
     */

    long FindTheBlockIndex(const char *index);

    /*
     * 从p块中删除data[num]
     * 会修改st，ed(if necessary)（文件和node_p都修改）
     * 会修改node_p和文件中的size
     * 会将num后的元素前移一位（文件和node_p都会修改）
     */

    void RemovePair(node &node_p,const long &p,const int &num);

    /*
     * 在一个block内插入target（插入为data[num]）
     * num <= node_p.size(在尾部添加时num == node_p.size)
     * 文件和node_p均修改：st,ed,size,data
     */

    void InsertPair(node &node_p,const long &p,const _pair &target,const int &num);

    /*
     * 插入：[st,ed]插入
     * 前一个的ed与后一个的st之间：插入后面
     * size==maxsize - 1时插入后裂开，size<maxsize - 1时直接插
     * 如果增加新节点，data数组必须开满，不足的用empty_pair填充
     * 增加一个新节点：直接在文件末尾增加————节点在文件中不按顺序存储！
     * 新增节点需考虑更新tail
     * 在文件开头创建第一个块（节点）
     * head 恒等于 0 (裂块向后裂，并块并到前一个)，保持前一个块不动
     * 若已存在与target一模一样的元素，则不插入
    */

    void insert(const char *index,const int &value);

    /*
     * 把node_q里的第take_num个元素借到node_p的第insert_num个元素位置
     * 同时修改node和文件
     */

    void BorrowElement(node &node_q,const long &q,node &node_p,const long &p,int take_num,int insert_num);

    /*
     * 并块：把p并到q的后面（q为p的前驱）
     * 更新tail (if necessary)
     * 同时修改node和文件
     */

    void Merge(node &node_p,const long &p,node &node_q,const long &q);

    /*
     * 删除以 [index] 为索引，值为 [value] 的条目。请注意，删除的条目可能不存在
     * 若删完后（--size后）size<MINSIZE，需要并块（==MINSIZE不并）
     *      具体逻辑：if (p == tail) : 向前驱借元素或并到前驱
     *              if (p == 0) 向后驱借元素或把后驱并过来 (保持head不变)
     *              else 向后驱借->向前驱借->并到前驱
     * head 恒等于 0 (裂块向后裂，并块并到前一个)，保持前一个块不变
     */

    void remove(const char *index,const int &value);

    /*
     * 找出所有索引为 [index] 的条目，并以 [value] 升序顺序输出，每个 [value] 间用一个空格隔开。
     * 若索引值为 [index] 的条目不存在，则输出 [null]。
     * 在整个指令输出结束后输出 \n
     */

    std::vector<int> find(const char *index);

};
```
## 文件存储说明
##### 录栈存于内存（登录状态的账户和登录顺序及其选择的书）
1. 一个 vector 内按登录顺序存 user_id, selected_book_index, privilege 
2. 另存一个 int 记录当前用户的 privilege
##### 已注册的所有用户存于外存
1. user_main.ull, user_other.ull 一个从 user_id 到 index 的块状链表
2. user.main 顺序存储所有用户的信息（每个用户位置为index）
3. log.user 顺序存储用户（user_id）和指令
##### 所有图书信息存于外存（块链）  
1. isbn_main.ull, isbn_other.ull 一个从 isbn 到 index 的块状链表
2. keyword_main.ull, keyword_other.ull 一个从 keyword 到 index 的块状链表
3. name_main.ull, name_other.ull 一个从 book_name 到 index 的块状链表
4. author_main.ull, author_other.ull 一个从 author 到 index 的块状链表
5. book.main 顺序存储所有图书的信息
6. log.fiance 顺序存储每一笔交易值（double）
## 其他补充说明
1. show命令的实现：ULL定义4个变量，分别存isbn,name,author,keyword的index
2. ULL中数据为pair<string,int>按string查找，得到对应的int后到文件对应位置修改或获取book/user的信息
2. 每本书、每个user有一个编号index(int)，按表示该组数据(book or user)存在对应文件的什么位置(long)
