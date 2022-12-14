#include "1774.h"

int main() {
    int n = 0;
    std::cin >> n;
    std::string command;
    BlockList the_list;
    while (n--) {
        std::cin >> command;
        if (command == "insert") {
            char index[64 * 8] = {0};
            int value;
            std::cin >> index >> value;
            the_list.insert(index,value);
        }
        else if (command == "delete") {
            char index[64 * 8] = {0};
            int value;
            std::cin >> index >> value;
            the_list.remove(index,value);
        }
        else if (command == "find") {
            char index[64 * 8] = {0};
            std::cin >> index;
            std::set<int> ans = the_list.find(index);
            if (ans.empty()) std::cout << "null" << std::endl;
            else {
                for (auto i : ans) {
                    std::cout << i << ' ';
                }
                std::cout << '\n';
            }
        }
    }
    return 0;
}
