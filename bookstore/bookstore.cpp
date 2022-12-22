#include "BookSystem.h"
#include "UserSystem.h"
#include "Command.h"
#include <iostream>
#include <iomanip>

int main () {
    BookSystem book_system;
    UserSystem user_system;
    std::cout << std::fixed << std::setprecision(2);
    int cnt = 0;

    std::string commandline;
    while (std::getline(std::cin,commandline)) {
//        std::cout << cnt << std::endl;
        try {
            if (processLine(commandline, user_system, book_system) == 0) break;
        }
        catch (Exception &x) {
            std::cout << "Invalid\n";
//            std::cout << x.getMessage() << std::endl;
        }
        ++cnt;
    }

    return 0;
}