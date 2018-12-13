#pragma once

#include <optional>
#include <sstream>
#include <vector>

namespace ecosnail::argo {

template <class T>
T cast(std::string_view text)
{
    std::stringstream stream;
    stream << text;
    T value;
    stream >> value;
    return value;
}

struct ArgumentData {
    // TODO: implement proper constructor? check state before virtual functions?
    virtual void provide(std::string_view value) = 0;

    std::vector<std::string> flags;
    bool multi = false;
    bool required = false;
    bool takesArgument = true;
    size_t timesUsed = 0;
    std::string helpText;
};

template <class T = void>
struct TypedArgumentData : ArgumentData {
    void provide(std::string_view value) override
    {
        values.push_back(cast<T>(value));
    }

    std::vector<T> values;
    std::optional<T> defaultValue;
};

template <>
struct TypedArgumentData<void> : ArgumentData {};

} // namespace::argo
