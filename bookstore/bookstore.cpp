#include "LogUser.h"
#include "LogFinance.h"
#include "BookSystem.h"
#include "UserSystem.h"
#include "Command.h"
#include <iostream>

int main () {
//    BookSystem book_system;
    UserSystem user_system;

    try {
        std::string commandline;
        while (std::cin >> commandline) {
            if (processLine(commandline,user_system) == 0) break;
        }
    }
    catch (Exception) {
        std::cout << "Invalid\n";
    }

    return 0;
}