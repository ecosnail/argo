#pragma once

#include <cassert>
#include <sstream>
#include <vector>
#include <memory>

namespace ecosnail::argo {

template <class T>
T cast(const std::string& text)
{
    std::stringstream stream;
    stream << text;
    T value;
    stream >> value;
    return value;
}

struct ArgumentData {
    // TODO: implement proper constructor? check state before virtual functions?
    virtual void provide(const std::string& value) = 0;

    std::vector<std::string> flags;
    bool multi = false;
    bool required = false;
    bool takesArgument = true;
    size_t timesUsed = 0;
    std::string helpText;
};

template <class T = void>
struct TypedArgumentData : ArgumentData {
    void provide(const std::string& value) override
    {
        values.push_back(cast<T>(value));
    }

    std::vector<T> values;
    std::unique_ptr<T> defaultValue;
};

template <>
struct TypedArgumentData<void> : ArgumentData {
    void provide(const std::string&) override
    {
        assert(false);
    }
};

} // namespace::argo
