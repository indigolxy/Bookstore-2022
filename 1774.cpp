#include <iostream>
#include <fstream>
#include <cstring>
#include <set>

constexpr int MaxBits = 64 * 8;
constexpr int MAXSIZE = 320; // !!1-base!!
constexpr int MINSIZE = 160;// !!1-base!!

class _pair {
public:
    char index[MaxBits] = {'\0'};
    int value;
    _pair(const char * _index,int value) : value(value){
        strcpy(index,_index);
    }
    _pair() : value(0) {}
};
class node {
    friend class BlockList;
    char st[MaxBits] = {'\0'};
    char ed[MaxBits] = {'\0'};
    int size; // !!1-base!!
    long prev;
    long next;
    _pair data[MAXSIZE];
public:
    node() : prev(-1),next(-1),size(1) {};
    node(int size,long prev,long next) : size(size),prev(prev),next(next) {}
};

class BlockList {
    bool flag_start = false;

public:
    BlockList() = default;
    ~BlockList() = default;

    static node ReadNode(std::fstream &file) {
        node tmp;
        file.read(tmp.st,MaxBits);
        file.read(tmp.ed,MaxBits);
        file.read(reinterpret_cast<char *> (&tmp.size),sizeof(int)); // ? size会自动变回int?
        file.read(reinterpret_cast<char *> (&tmp.prev),sizeof(long));
        file.read(reinterpret_cast<char *> (&tmp.next),sizeof(long));

        for (int i = 0;i < tmp.size;++i) {
            file.read(tmp.data[i].index,MaxBits);
            file.read(reinterpret_cast<char *> (&tmp.data[i].value),sizeof(int));
        }
        return tmp;
    }

    /*
     * 写入一个pair并将写指针移动至该pair的结尾
     * 常在循环中调用，用于写入data
     */

    static void WritePair(std::fstream &file,const _pair &x) {
        file.write(x.index,MaxBits);
        file.write(reinterpret_cast<const char *> (&x.value),sizeof(int));
    }

    static void WriteHalfNode(std::fstream &file,const char * st,const char * ed,const int &size,const long &prev,const long &next) {
        file.write(st,MaxBits); // st
        file.write(ed,MaxBits); // ed
        file.write(reinterpret_cast<const char *> (&size),sizeof(int)); // size
        file.write(reinterpret_cast<const char *> (&prev),sizeof(long)); // prev = p
        file.write(reinterpret_cast<const char *> (&next),sizeof(long)); // next = p.next
    }

    /*
     * 插入：[st,ed]插入
     * 前一个的ed与后一个的st之间：插入后面
     * size==maxsize - 1时插入后裂开，size<maxsize - 1时直接插
     * 如果增加新节点，data数组必须开满，不足的用empty_pair填充
     * 增加一个新节点：直接在文件末尾增加————节点在文件中不按顺序存储！
     * 在文件开头创建第一个块（节点）
     * head 恒等于 0 (裂块向后裂，并块并到前一个)，保持前一个块不动
    */

    void _insert(const char * &index,const int &value,std::fstream &file) {
        if (!flag_start) {
            // 第一次insert:the very first _pair data
            // 在文件开头创建第一个块（节点）
            // head 恒等于 0 (裂块向后裂，并块并到前一个)，保持前一个块不动
            file.seekp(std::ios::beg);
            WriteHalfNode(file,index,index,1,-1,-1);
            WritePair(file,{index,value});
            _pair  empty_pair;
            for (int i = 1;i < MAXSIZE;++i) {
                WritePair(file,empty_pair);
            }
            flag_start = true;
            return;
        }
        long p = 0;
        file.seekg(p);
        node node_p = ReadNode(file); // 读指针到了data的最后
        while (node_p.next != -1) {
            if (std::strcmp(index,node_p.ed) > 0) { // >ed：看下一个节点
                p = node_p.next;
                file.seekg(p);
                node_p = ReadNode(file); // 读指针到了data的最后(size处)
                continue;
            }
            else { // 第一个<=ed的节点：插入
                // 先直接插
                int i = 0;
                while (std::strcmp(index,node_p.data[i].index) > 0) { ++i; }
                // 插在i之前
                if (i == 0) {
                    // 修改p.st
                    file.seekp(p);
                    file.write(index,MaxBits);
                }
                // p->data[i] = {index,value};
                file.seekp(MaxBits + sizeof(int) + 2 * sizeof(long) + i * sizeof(_pair),std::ios::cur);
                file.write(index,MaxBits);
                file.write(reinterpret_cast<const char *> (&value),sizeof(int));

                // 原本的i到结尾，每个元素后移一位
                for (int j = i;j < node_p.size;++j) {
                    WritePair(file,node_p.data[j]);
                }

                // ++p->size;
                file.seekp(p + MaxBits * 2);
                ++node_p.size;
                file.write(reinterpret_cast<const char *> (&node_p.size),sizeof(int));

                // 再裂开:向后增加
                if (node_p.size == MAXSIZE) {
                    // 增加一个新节点：直接在文件末尾增加————节点在文件中不按顺序存储！
                    file.seekp(std::ios::end);
                    const long q = file.tellp();
                    WriteHalfNode(file,node_p.data[MINSIZE].index,node_p.data[MAXSIZE - 1].index,MINSIZE,p,node_p.next);
                    for (int j = MINSIZE;j < MAXSIZE;++j) {
                        WritePair(file,node_p.data[j]);
                    }
                    _pair empty_pair;
                    for (int j = MINSIZE;j < MAXSIZE;++j) {
                        WritePair(file,empty_pair);
                    }

                    // 修改p中数据
                    file.seekp(p + MaxBits);
                    file.write(node_p.data[MINSIZE - 1].index,MaxBits); // strcpy(p->ed,p->data[MINSIZE - 1].index);
                    file.write(reinterpret_cast<const char *> (&MINSIZE),sizeof(int)); // p->size = MINSIZE;
                    file.seekp(sizeof(long),std::ios::cur); // 跳过prev
                    file.write(reinterpret_cast<const char *> (&q),sizeof(long)); // p->next = q;
                    file.seekp(MINSIZE * sizeof(_pair),std::ios::cur); // 跳过前一半data
                    for (int j = MINSIZE;j < MAXSIZE;++j) {
                        WritePair(file,empty_pair);
                    }

                    // p->next->prev = q;
                    file.seekp(node_p.next + 2 * MaxBits + sizeof(int));
                    file.write(reinterpret_cast<const char *> (&q),sizeof(long));
                }
                return; // 插入完成！
            }
        }
        // 遍历到最后都不>每一个ed：插入最后一个节点或者新增一个节点
        if (node_p.size < MAXSIZE - 1)  {
            // 插入最后一个节点
            file.seekp(file.tellg());
            WritePair(file,{index,value}); // p->data[p->size] = {index,value};
            file.seekp(p + MaxBits);
            file.write(index,MaxBits); // strcpy(p->ed,index);
            ++node_p.size;
            file.write(reinterpret_cast<const char *> (&node_p.size),sizeof(int)); // ++p->size;
            return;
        }
        else {
            // 在最后新增一个节点(不是块状链表的第一个节点——有prev无next)
            file.seekp(p + 2 * MaxBits + sizeof(int) + sizeof(long));
            file.write(reinterpret_cast<const char *> (&std::ios::end),sizeof(long)); // p->next = q;

            file.seekp(std::ios::end); // 在末尾新增节点
            WriteHalfNode(file,index,index,1,p,-1);
            WritePair(file,{index,value});
            _pair  empty_pair;
            for (int i = 1;i < MAXSIZE;++i) {
                WritePair(file, empty_pair);
            }
            return;
        }
    }

    /*
     * 删除以 [index] 为索引，值为 [value] 的条目。请注意，删除的条目可能不存在
     * head 恒等于 0 (裂块向后裂，并块并到前一个)，保持前一个块不动
     *
     */

    void _delete(const char * &index,const int &value) {
        // todo 删除以 [index] 为索引，值为 [value] 的条目。请注意，删除的条目可能不存在


    }

    std::set<int> _find(const char * &index) {
        // todo 找出所有索引为 [index] 的条目，并以 [value] 升序顺序输出，每个 [value] 间用一个空格隔开。若索引值为 [index] 的条目不存在，则输出 [null]。在整个指令输出结束后输出 \n
    }
};

int main() {
    int n = 0;
    std::cin >> n;
    std::string command;
    std::fstream file("file.txt");
    BlockList the_list;
    while (n--) {
        std::cin >> command;
        if (command == "insert")
    }
    file.close();

}