#pragma once

#include <exception>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace ecosnail::argo {

class Exception : public std::exception {};

namespace exception {

class FlagException : public Exception {
public:
    FlagException(
        const std::string& message, const std::vector<std::string>& flags);
    FlagException(const std::string& message, std::string_view flag);

    const char* what() const noexcept override;

private:
    std::string _what;
};

class FlagDoesNotHaveKeys : public Exception {};

class FlagDuplicated : public FlagException {
public:
    FlagDuplicated(std::string_view flag);
};

class ArgumentsAfterMultiArgument : public Exception {};
class RequiredNotConstructible : public Exception {};
class SubsequentParseCall : public Exception {};
class ValueNotConvertibleToString : public Exception {};

} // namespace exception

} // namespace ecosnail::argo
