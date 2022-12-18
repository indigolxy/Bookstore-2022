#include "1774.h"

int main() {
    int n = 0;
    std::cin >> n;
    std::string command;
    BlockList the_list;
    while (n--) {
        std::cin >> command;
        char index[MaxBits] = {0};
        if (command == "insert") {
            int value;
            std::cin >> index >> value;
            the_list.insert(index,value);
        }
        else if (command == "delete") {
            int value;
            std::cin >> index >> value;
            the_list.remove(index,value);
        }
        else if (command == "find") {
            std::cin >> index;
            std::vector<int> ans = the_list.find(index);

            if (ans.empty()) std::cout << "null" << std::endl;
            else {
                for (auto i : ans) {
                    std::cout << i << ' ';
                }
                std::cout << '\n';
            }
        }
//        if (n == 479 || n == 480 || n == 481) std::cout << "STATUS: " << n << ' ' << bool(the_list.file) << std::endl;
//        the_list.printAll();
    }
    return 0;
}
