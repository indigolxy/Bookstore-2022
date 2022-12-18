#include "1774.h"

bool _pair::operator==(const _pair &obj) {
    if (strcmp(index,obj.index) == 0 && value == obj.value) return true;
    else return false;
}

bool _pair::operator<(const _pair &obj) {
    if (strcmp(index,obj.index) < 0) return true;
    else if (strcmp(index,obj.index) > 0) return false;
    else {
        if (value < obj.value) return true;
        else return false;
    }
}

BlockList::BlockList() {
    file.open("other.txt",std::ifstream::in | std::ifstream::binary);
    if (file.good()) { // 文件存在 需要读入
        file.seekg(std::ios::beg);
        file.read(reinterpret_cast<char *> (&flag_start),sizeof(bool));
        file.read(reinterpret_cast<char *> (&tail),sizeof(long));
    }
    file.close();
    file.clear();
    file.open("file.txt",std::fstream::app | std::fstream::binary);
    file.close();
    file.clear();
    file.open("file.txt",std::fstream::in | std::fstream::out | std::fstream::binary);
}

BlockList::~BlockList() {
    file.close();
    file.clear();
    file.open("other.txt",std::ofstream::out | std::ofstream::binary);
    file.write(reinterpret_cast<const char *> (&flag_start),sizeof(bool));
    file.write(reinterpret_cast<const char *> (&tail),sizeof(long));
    file.close();
    file.clear();
}

node BlockList::ReadNode(const long &pos) { // 读完读指针走到data有效数据的最后
    file.seekg(pos);
    node tmp;
    file.read(reinterpret_cast<char *>(&tmp), sizeof(node));
    return tmp;
}

void BlockList::WriteNode(const node &obj,const long &pos) {
    if (pos == -1) file.seekp(0,std::ios::end);
    else file.seekp(pos);
    file.write(reinterpret_cast<char const*>(&obj), sizeof(node));
}

long BlockList::FindTheBlock(const _pair &target) {
    long p = 0;
    while (true) {
        file.seekg(p + sizeof(_pair));
        _pair p_ed;
        file.read(reinterpret_cast<char *> (&p_ed),sizeof(_pair));
        if (p_ed >= target) {
            return p;
        }
        else {
            if (p == tail) return -1;
            // p = node_p.next;
            file.seekg(p + sizeof(_pair) * 2 + sizeof(int) + sizeof(long));
            file.read(reinterpret_cast<char *> (&p),sizeof(long));
        }
    }
}

long BlockList::FindTheBlockIndex(const char *index) {
    long p = 0;
    while (true) {
        file.seekg(p + sizeof(_pair));
        char p_ed_index[MaxBits] = {0};
        file.read(reinterpret_cast<char *> (&p_ed_index),MaxBits);
        if (strcmp(index,p_ed_index) <= 0) return p;
        if (p == tail) return -1;
        // p = node_p.next;
        file.seekg(p + sizeof(_pair) * 2 + sizeof(int) + sizeof(long));
        file.read(reinterpret_cast<char *> (&p),sizeof(long));
    }
}

void BlockList::RemovePair(node &node_p,const long &p,const int &num) {
    if (num == 0 && num == node_p.size - 1) {
        _pair empty_pair;
        node_p.st = empty_pair;
        node_p.ed = empty_pair;
        if (p == 0) flag_start = false;
    }
    if (num == 0) {
        node_p.st = node_p.data[1];
    }
    else if (num == node_p.size - 1) {
        node_p.ed = node_p.data[node_p.size - 2];
    }

    --node_p.size;
    for (int i = num;i < node_p.size;++i) {
        node_p.data[i] = node_p.data[i + 1];
    }
    _pair empty_pair;
    node_p.data[node_p.size] = empty_pair;

    WriteNode(node_p,p);
}

void BlockList::InsertPair(node &node_p,const long &p,const _pair &target,const int &num) {
    if (num == 0) {
        node_p.st = target;
    }
    else if (num == node_p.size) {
        node_p.ed = target;
    }
    for (int i = node_p.size;i > num;--i) {
        node_p.data[i] = node_p.data[i - 1];
    }
    node_p.data[num] = target;
    ++node_p.size;

    WriteNode(node_p,p);
}

void BlockList::insert(char *index,const int &value) {
    const _pair target(index,value);
    if (!flag_start) {
        // 第一次insert:the very first _pair data
        // 在文件开头创建第一个块（节点）
        node tmp(target,target,1,-1,-1);
        tmp.data[0] = target;
        WriteNode(tmp,0);

        flag_start = true;
        tail = 0;
        return;
    }

    long p = FindTheBlock(target);
    if (p == -1) p = tail; // 遍历到最后都>每一个ed：插入最后一个节点或者新增一个节点 相当于插入最后一个节点（或许再裂块
    node node_p = ReadNode(p);

    // 先直接插
    int i = 0;
    while (node_p.data[i] < target && i < node_p.size) {
        ++i;
    }
    if (node_p.data[i] == target) return; // 若重复，不插入
    InsertPair(node_p,p,target,i);

    // 再裂开:向后增加 (考虑更新tail）
    if (node_p.size == MAXSIZE) {
        // 增加一个新节点：直接在文件末尾增加————节点在文件中不按顺序存储！
        node tmp(node_p.data[MINSIZE],node_p.data[MAXSIZE - 1],MINSIZE,p,node_p.next);
        for (int j = MINSIZE;j < MAXSIZE;++j) {
            tmp.data[j - MINSIZE] = node_p.data[j];
        }
        file.seekp(0, std::ios::end);
        const long q = file.tellp();
        WriteNode(tmp,q);

        long p_next_original = node_p.next;

        // 修改p中数据
        node_p.ed = node_p.data[MINSIZE - 1];
        node_p.size = MINSIZE;
        node_p.next = q;
        _pair empty_pair;
        for (int j = MINSIZE;j < MAXSIZE;++j) {
            node_p.data[j] = empty_pair;
        }
        WriteNode(node_p,p);

        if (p == tail) tail = q;
        else {
            // p->next->prev = q;
            file.seekp(p_next_original + 2 * sizeof(_pair) + sizeof(int));
            file.write(reinterpret_cast<const char *> (&q), sizeof(long));
        }
    }
}

void BlockList::BorrowElement(node &node_q,const long &q,node &node_p,const long &p,int take_num,int insert_num) {
    _pair borrow = node_q.data[take_num];
    RemovePair(node_q,q,take_num);
    InsertPair(node_p,p,borrow,insert_num);
}

void BlockList::Merge(node &node_p,const long &p,node &node_q,const long &q) {
    // 先修改前驱数据
    node_q.ed = node_p.ed;
    node_q.next = node_p.next;
    for (int i = 0;i < node_p.size;++i) {
        node_q.data[node_q.size + i] = node_p.data[i];
    }
    node_q.size = node_q.size + node_p.size;
    WriteNode(node_q,q);

    // 再修改node_p.next.prev = q
    if (p == tail) tail = q;
    else {
        file.seekp(node_p.next + 2 * sizeof(_pair) + sizeof(int));
        file.write(reinterpret_cast<const char *> (&q),sizeof(long));
    }
}

void BlockList::remove(char *index,const int &value) {
    if (!flag_start) return;

    const _pair target(index,value);
    long p = FindTheBlock(target);
    if (p == -1) return;
    node node_p = ReadNode(p);
    if (node_p.st > target || node_p.ed < target) return; // target不存在
    int num_of_target = -1;
    for (int i = 0;i < node_p.size;++i) {
        if (node_p.data[i] == target) { // found it!
            num_of_target = i;
            break;
        }
    }
    if (num_of_target  == -1) return; // target不存在

    RemovePair(node_p,p,num_of_target); // node_p也完全修改

    if (node_p.size >= MINSIZE) return;
    if (p == tail && p == 0) return; // 只有一个块，不并

    // 开始借元素or并块
    if (p == 0) {
        long q_next = node_p.next;
        node node_q_next = ReadNode(q_next);
        if (node_q_next.size > MINSIZE) {
            BorrowElement(node_q_next, q_next, node_p, p, 0, node_p.size);
            return;
        }
        Merge(node_q_next,q_next,node_p,p);
        return;
    }
    if (p == tail) {
        long q_prev = node_p.prev;
        node node_q_prev = ReadNode(q_prev);
        if (node_q_prev.size > MINSIZE) {
            BorrowElement(node_q_prev,q_prev,node_p,p,node_q_prev.size - 1,0);
            return;
        }
        Merge(node_p,p,node_q_prev,q_prev);
        return;
    }
    long q_next = node_p.next;
    node node_q_next = ReadNode(q_next);
    if (node_q_next.size > MINSIZE) {
        BorrowElement(node_q_next,q_next,node_p,p,0,node_p.size);
        return;
    }
    long q_prev = node_p.prev;
    node node_q_prev = ReadNode(q_prev);
    if (node_q_prev.size > MINSIZE) {
        BorrowElement(node_q_prev,q_prev,node_p,p,node_q_prev.size - 1,0);
        return;
    }
    Merge(node_p,p,node_q_prev,q_prev);
    return;
}

std::vector<int> BlockList::find(char *index) {
    std::vector<int> ans;
    if (!flag_start) return ans;
    long p = FindTheBlockIndex(index);
    if (p == -1) return ans;

    node node_p = ReadNode(p);
    if (strcmp(index,node_p.st.index) < 0) return ans; // index不存在

    bool found_index = false;
    for (int i = 0;i < node_p.size;++i) {
        if (strcmp(node_p.data[i].index,index) == 0) { // found it!
            found_index = true;
            ans.push_back(node_p.data[i].value);
        }
        else if (found_index) {
            return ans; // 在这个block内找完了
        }
    }
    if (!found_index) return ans; // index不存在

    while (true) {
        if (p == tail) return ans;
        p = node_p.next;
        node_p = ReadNode(p);
        for (int i = 0;i < node_p.size;++i) {
            if (strcmp(node_p.data[i].index,index) == 0) {
                ans.push_back(node_p.data[i].value);
            }
            else {
                return ans; // 在这个block内找完了
            }
        }
    }
}