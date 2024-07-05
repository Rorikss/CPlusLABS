//
// Created by Roriks on 3/22/2024.
//
#pragma once
#include <iostream>
#include <iterator>

namespace adapters {

// check if iterator is reversible
template<typename Iterator>
constexpr bool IsReversible() {
        return std::is_base_of_v<std::bidirectional_iterator_tag, typename Iterator::iterator_category>;
}

template<typename Iterator>
constexpr bool IsForwardBased() {
    return std::is_base_of_v<std::forward_iterator_tag, typename Iterator::iterator_category>;
}

// iterators for adapters
template<typename Iterator, typename UnaryFunction>
class TransformIterator {
public:
    using iterator_category = std::conditional_t<IsReversible<Iterator>(), std::bidirectional_iterator_tag, std::forward_iterator_tag>;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;

    TransformIterator(Iterator it, Iterator end, UnaryFunction func)
            : it_(it), end_(end), func_(func) {}

    auto operator*() {
        return func_(*it_);
    }

    TransformIterator& operator++() {
        ++it_;
        return *this;
    }

    TransformIterator operator++(int) {
        TransformIterator old = *this;
        operator++();
        return old;
    }

    template<typename T = Iterator, typename = std::enable_if_t<IsReversible<T>()>>
    TransformIterator& operator--() {
        --it_;
        return *this;
    }

    template<typename T = Iterator, typename = std::enable_if_t<IsReversible<T>()>>
    TransformIterator operator--(int) {
        TransformIterator old = *this;
        operator--();
        return old;
    }

    bool operator==(const TransformIterator& other) const { return it_ == other.it_; }

    bool operator!=(const TransformIterator& other) const { return !(*this == other); }

private:
    Iterator it_;
    Iterator end_;
    UnaryFunction func_;
};

template<typename Iterator, typename Predicate>
class FilterIterator {
public:
    using iterator_category = std::conditional_t<IsReversible<Iterator>(), std::bidirectional_iterator_tag, std::forward_iterator_tag>;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;


    FilterIterator(Iterator it, Iterator end, Predicate pred)
            : it_(it), begin_(it), end_(end), pred_(pred) {
        while (it_ != end_ && !pred_(*it_)) {
            ++it_;
        }
    }

    auto operator*() {
        return *it_;
    }

    FilterIterator& operator++() {
        do {
            ++it_;
        } while (it_ != end_ && !pred_(*it_));
        return *this;
    }

    FilterIterator operator++(int) {
        FilterIterator old = *this;
        operator++();
        return old;
    }

    template<typename T = Iterator, typename = std::enable_if_t<IsReversible<T>()>>
    FilterIterator& operator--() {
        do {
            --it_;
        } while (it_ != begin_ && !pred_(*it_));
        return *this;
    }

    template<typename T = Iterator, typename = std::enable_if_t<IsReversible<T>()>>
    FilterIterator operator--(int) {
        FilterIterator old = *this;
        operator--();
        return old;
    }

    bool operator==(const FilterIterator& other) const { return it_ == other.it_; }

    bool operator!=(const FilterIterator& other) const { return !(*this == other); }

private:
    Iterator it_;
    Iterator begin_;
    Iterator end_;
    Predicate pred_;
};

template<typename Iterator>
class TakeAndDropIterator {
public:
    using iterator_category = std::conditional_t<IsReversible<Iterator>(), std::bidirectional_iterator_tag, std::forward_iterator_tag>;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;

    TakeAndDropIterator(Iterator it)
            : it_(it) {}

    auto operator*() {
        return *it_;
    }

    TakeAndDropIterator& operator++() {
        ++it_;
        return *this;
    }

    TakeAndDropIterator operator++(int) {
        TakeAndDropIterator old = *this;
        operator++();
        return old;
    }

    template<typename T = Iterator, typename = std::enable_if_t<IsReversible<T>()>>
    TakeAndDropIterator& operator--() {
        --it_;
        return *this;
    }

    template<typename T = Iterator, typename = std::enable_if_t<IsReversible<T>()>>
    TakeAndDropIterator operator--(int) {
        TakeAndDropIterator old = *this;
        operator--();
        return old;
    }

    bool operator==(const TakeAndDropIterator& other) const { return it_ == other.it_; }

    bool operator!=(const TakeAndDropIterator& other) const { return !(*this == other); }

private:
    Iterator it_;
};

template<typename Iterator>
class ReverseIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;

    ReverseIterator(Iterator it)
            : it_(it) {}

    auto operator*() {
        return *(std::prev(it_));
    }

    ReverseIterator& operator++() {
        --it_;
        return *this;
    }

    ReverseIterator operator++(int) {
        ReverseIterator old = *this;
        operator++();
        return old;
    }

    ReverseIterator& operator--() {
        ++it_;
        return *this;
    }

    ReverseIterator operator--(int) {
        ReverseIterator old = *this;
        operator--();
        return old;
    }

    bool operator==(const ReverseIterator& other) const { return it_ == other.it_; }

    bool operator!=(const ReverseIterator& other) const { return !(*this == other); }

private:
    Iterator it_;
};
}
