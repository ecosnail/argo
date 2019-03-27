#pragma once

#include <cassert>
#include <memory>
#include <sstream>
#include <type_traits>
#include <vector>

namespace ecosnail::argo {

template <class T, class = void>
struct Cast {
    T operator()(const std::string& text)
    {
        std::istringstream stream(text);
        T value;
        stream >> value;
        return value;
    }
};

template <class T>
struct Cast<
        T,
        typename std::enable_if<
            std::is_convertible<std::string, T>::value
        >::type>  {
    T operator()(const std::string& text)
    {
        return text;
    }
};

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
        values.push_back(Cast<T>{}(value));
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
