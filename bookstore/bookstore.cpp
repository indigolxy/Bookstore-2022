#include "LogUser.h"
#include "LogFinance.h"
#include "BookSystem.h"
#include "UserSystem.h"
#include "Command.h"
#include <iostream>

int main () {
//    BookSystem book_system;
    UserSystem user_system;

    std::string commandline;
    while (std::getline(std::cin,commandline)) {
        try {
            if (processLine(commandline,user_system) == 0) break;
        }
        catch (Exception &x) {
            std::cout << "Invalid\n" << x.getMessage() << std::endl;
        }
    }

    return 0;
}