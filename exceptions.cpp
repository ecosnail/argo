#include <ecosnail/argo/exceptions.hpp>

namespace ecosnail::argo::exception {

namespace {

std::ostream& joinStrings(
    std::ostream& output, const std::vector<std::string>& strings)
{
    for (auto it = strings.begin(); it != strings.end(); ) {
        output << *it++;
        for (; it != strings.end(); ++it) {
            output << ", " << *it;
        }
    }
    return output;
}

} // namespace

FlagException::FlagException(
    const std::string& message,
    const std::vector<std::string>& flags)
{
    std::ostringstream stream;
    stream << message;
    for (auto it = flags.begin(); it != flags.end(); ) {
        stream << ": " << *it++;
        for (; it != flags.end(); ++it) {
            stream << ", " << *it;
        }
    }
    _what = stream.str();
}

FlagException::FlagException(
    const std::string& message, const std::string& flag)
{
    std::ostringstream stream;
    stream << message << ": " << flag;
    _what = stream.str();
}

const char* FlagException::what() const noexcept
{
    return _what.c_str();
}

FlagDuplicated::FlagDuplicated(const std::string& flag)
    : FlagException("flag duplicated", flag)
{ }

} // namespace ecosnail::argo::exception
