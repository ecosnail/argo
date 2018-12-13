#pragma once

#include <ecosnail/argo/shared_data.hpp>
#include <ecosnail/argo/argument_iterator.hpp>

#include <cassert>
#include <iterator>
#include <memory>
#include <vector>
#include <sstream>
#include <optional>

namespace ecosnail::argo {

template <class T>
class Argument {
    using Values = std::vector<T>;

public:
    explicit Argument(std::shared_ptr<TypedArgumentData<T>> data)
        : _data(std::move(data))
    { }

    operator T() const
    {
        // TODO: think about this assert
        assert(!_data->multi);

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
        return ArgumentIterator<T>(_data->values.begin());
    }

    auto end() const
    {
        return ArgumentIterator<T>(_data->values.end());
    }

    Argument help(std::string_view helpText)
    {
        _data->helpText = helpText;
        return *this;
    }

    Argument required()
    {
        _data->required = true;
        return *this;
    }

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
