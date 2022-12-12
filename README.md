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

### UserSystem.h/UserSystem.cpp

### BookSystem.h/BookSystem.cpp

### UnrolledLinkList.h/UnrolledLinkList.cpp

### LogUser.h/LogUser.cpp

### LogFinance.h/LogFinance.cpp


## 文件存储说明
登录栈存于内存（登录状态的账户和登录顺序及其选择的书）  
已注册的所有用户存于外存（块链）  
所有图书信息存于外存（块链）  
日志-交易信息、操作记录分两个文件存于外存
## 其他补充说明
1. show命令的实现：ULL定义4个变量，分别存isbn,name,author,keyword的index
2. ULL中数据为pair<string,int>按string查找，得到对应的int后到文件对应位置修改或获取book/user的信息
2. 每本书、每个user有一个编号index(int)，按先后顺序表示该组数据(book or user)存在对应文件的第几行
