#pragma once

#include <iterator>
#include <vector>

namespace ecosnail::argo {

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
