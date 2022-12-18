cmake_minimum_required(VERSION 3.22)
project(Bookstore-2022)

set(CMAKE_CXX_STANDARD 14)

add_executable(code bookstore/bookstore.cpp
        bookstore/UserSystem.cpp
        bookstore/BookSystem.cpp
        bookstore/LogFinance.cpp
        bookstore/LogUser.cpp
        bookstore/Command.cpp
        bookstore/UnrolledLinkList.cpp
        )

add_executable(test 1774/1774.cpp 1774/1774test.cpp)