//
// Created by 李心瑶 on 2022/12/18.
//

#ifndef BOOKSTORE_2022_ERROR_H
#define BOOKSTORE_2022_ERROR_H

#include <exception>
#include <string>

class Exception : public std::exception {
private:
    std::string message;
public:
    Exception() = default;
    explicit Exception(std::string _message) : message(std::move(_message)) {}
    const char *what() const noexcept override {return "Invalid.\n";}
    std::string getMessage() {return message;}
};

#endif //BOOKSTORE_2022_ERROR_H
