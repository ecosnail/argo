#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace ecosnail::argo {

class Exception : public std::exception {
public:
    Exception(std::string_view message = "")
        : _message(message)
    { }

    const char* what() const noexcept override
    {
        return _message.c_str();
    }

private:
    std::string _message;
};

inline void check(bool condition, std::string_view message = "")
{
    if (!condition) {
        throw Exception(message);
    }
}

} // namespace ecosnail::argo
