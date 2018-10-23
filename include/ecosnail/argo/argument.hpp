#pragma once

#include <ecosnail/argo/errors.hpp>

#include <iterator>
#include <memory>
#include <vector>
#include <sstream>
#include <optional>

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

template <class T>
struct TypedArgumentData : ArgumentData {
    void provide(std::string_view value) override
    {
        values.push_back(cast<T>(value));
    }

    std::vector<T> values;
    std::optional<T> defaultValue;
};

template <class T>
class ArgumentIterator {
public:
    explicit ArgumentIterator(
            typename std::vector<T>::const_iterator dataIterator)
        : _dataIterator(std::move(dataIterator))
    { }

    const T& operator*() const
    {
        return *_dataIterator;
    }

    ArgumentIterator& operator++()
    {
        ++_dataIterator;
        return *this;
    }

    ArgumentIterator operator++(int)
    {
        ArgumentIterator previous(*this);
        operator++();
        return previous;
    }

    friend bool operator!=(
        const ArgumentIterator& lhs, const ArgumentIterator& rhs)
    {
        return lhs._dataIterator != rhs._dataIterator;
    }

private:
    typename std::vector<T>::const_iterator _dataIterator;
};

// TODO: more informative error messages

template <class T>
class Argument {
    using Values = std::vector<T>;

public:
    explicit Argument(std::shared_ptr<TypedArgumentData<T>> data)
        : _data(std::move(data))
    { }

    operator T() const
    {
        check(!_data->multi, "cannot cast multi argument to a single value");
        // TODO: check default-constructible somewhere?
        if (!_data->values.empty()) {
            return _data->values.front();
        }
        if (_data->defaultValue) {
            return *_data->defaultValue;
        }
        return T{};
    }

    auto begin() const
    {
        check(_data->multi, "cannot iterate over a non-multi argument");
        return ArgumentIterator<T>(_data->values.begin());
    }

    auto end() const
    {
        check(_data->multi, "cannot iterator over a non-multi argument");
        return ArgumentIterator<T>(_data->values.end());
    }

    Argument help(std::string_view helpText)
    {
        _data->helpText = helpText;
        return *this;
    }

    Argument required()
    {
        // TODO: check default value not present?
        _data->required = true;
        return *this;
    }

    // TODO: check default-constructible or required?

    Argument defaultValue(T value)
    {
        _data->defaultValue = value;
        return *this;
    }

    Argument multi()
    {
        _data->multi = true;
        return *this;
    }

    Argument flag()
    {
        _data->takesArgument = false;
        return *this;
    }

    friend std::ostream& operator<<(
        std::ostream& output, const Argument& argument)
    {
        if (argument._data->multi) {
            if (auto it = argument.begin(); it != argument.end()) {
                output << *it++;
                for (; it != argument.end(); ++it) {
                    output << ", " << *it;
                }
            }
        } else {
            output << argument.operator T();
        }

        return output;
    }

private:
    std::shared_ptr<TypedArgumentData<T>> _data;
};

} // namespace ecosnail::argo

namespace std {

template <class T>
struct iterator_traits<ecosnail::argo::ArgumentIterator<T>> {
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = input_iterator_tag;
};

} // namespace std
