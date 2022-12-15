
#ifndef BOOKSTORE_2022_1774_H
#define BOOKSTORE_2022_1774_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <set>

constexpr int MaxBits = 64 * 8;
constexpr int MAXSIZE = 320; // !!1-base!!
constexpr int MINSIZE = 160; // !!1-base!!

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

class BlockList {
//    friend int main();
    std::fstream file; // "other.txt" "file.txt"
    bool flag_start = false;
    long tail = 0;

public:
    BlockList();

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

    void insert(char *index,const int &value);

    /*
     * 删除以 [index] 为索引，值为 [value] 的条目。请注意，删除的条目可能不存在
     * 若删完后（--size后）size<MINSIZE，需要并块（==MINSIZE不并）
     * head 恒等于 0 (裂块向后裂，并块并到前一个)，保持前一个块不变
     * 并块记得更新tail
     */

    void remove(char *index,const int &value);

    /*
     * 找出所有索引为 [index] 的条目，并以 [value] 升序顺序输出，每个 [value] 间用一个空格隔开。
     * 若索引值为 [index] 的条目不存在，则输出 [null]。
     * 在整个指令输出结束后输出 \n
     */

    std::set<int> find(char *index);

};

#endif //BOOKSTORE_2022_1774_H
