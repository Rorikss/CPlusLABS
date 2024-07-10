//
// Created by Roriks on 3/22/2024.
//

#pragma once
#include <iostream>
#include <iterator>
#include "Iterators.h"

namespace adapters {
template <typename Container>
concept IsAssociative = requires(Container c) {
    typename Container::key_type;
};

template <typename Container>
concept IsNodeContainer = requires(Container c) {
    typename Container::mapped_type;
};

// range container implementation
template<typename Iterator, typename RangeIterator, typename Container, typename Functor = std::nullptr_t>
class RangeContainer {
public:
    RangeContainer(Iterator begin, Iterator end, const Container& cont, Functor func)
            : begin_(begin)
            , end_(end)
            , cont_(cont)
            , func_(func)
    {
        DoStaticAsserts();
    }

    RangeContainer(Iterator begin, Iterator end, Container cont)
            : begin_(begin)
            , end_(end)
            , cont_(cont)
    {
        DoStaticAsserts();
    }

    using size_type = decltype(std::declval<Container>().size());
    using iterator = RangeIterator;

    iterator begin() const {
        if constexpr (std::is_same_v<Functor, std::nullptr_t>) {
            return iterator(begin_);
        } else {
            return iterator(begin_, end_, func_);
        }
    }
    iterator end() const {
        if constexpr (std::is_same_v<Functor, std::nullptr_t>) {
            return iterator(end_);
        } else {
            return iterator(end_, end_, func_);
        }
    }

    size_type size() {
        return cont_.size();
    }

private:
    void DoStaticAsserts() {
        static_assert(ForwardIterator<Iterator>, "Iterator should be at least forward!");
        static_assert(!IsNodeContainer<std::remove_reference_t<Container>>, "unable to work with pair values!");
    }

    Iterator begin_;
    Iterator end_;
    Functor func_;
    Container& cont_;
};

// associative containers and view on them
// definition
class keys;
class values;

template <typename Iterator, typename Container, typename Functor>
class AssociativeRange {
public:
    class AssociativeIterator {
    public:
        using iterator_category = std::conditional_t<ReversibleIterator<Iterator>, std::bidirectional_iterator_tag, std::forward_iterator_tag>;
        using difference_type = typename std::iterator_traits<Iterator>::difference_type;
        using value_type = typename std::iterator_traits<Iterator>::value_type;
        using pointer = typename std::iterator_traits<Iterator>::pointer;
        using reference = typename std::iterator_traits<Iterator>::reference;

        AssociativeIterator(Iterator it)
                : it_(it)
                {}

        auto operator*() {
            if constexpr (IsNodeContainer<std::remove_reference_t<Container>>) {
                if constexpr (std::is_same_v<Functor, keys>) {
                    return it_->first;
                } else if constexpr (std::is_same_v<Functor, values>) {
                    return it_->second;
                }
            } else {
                return *it_;
            }
        }

        AssociativeIterator& operator++() {
            ++it_;
            return *this;
        }

        AssociativeIterator operator++(int) {
            AssociativeIterator old = *this;
            operator++();
            return old;
        }

        template<typename T = Iterator, typename = std::enable_if_t<ReversibleIterator<T>>>
        AssociativeIterator& operator--() {
            --it_;
            return *this;
        }

        template<typename T = Iterator, typename = std::enable_if_t<ReversibleIterator<T>>>
        AssociativeIterator operator--(int) {
            AssociativeIterator old = *this;
            operator--();
            return old;
        }

        bool operator==(const AssociativeIterator& other) const { return it_ == other.it_; }
        bool operator!=(const AssociativeIterator& other) const { return !(*this == other); }

    private:
        Iterator it_;
    };

    using iterator = AssociativeIterator;
    using const_iterator = const iterator;
    using size_type = decltype(std::declval<Container>().size());

    AssociativeRange(const Container& cont, Iterator begin, Iterator end)
            : begin_(begin)
            , end_(end)
            , cont_(cont)
    {
        DoStaticAsserts();
    }

    iterator begin() const { return iterator(begin_); }
    iterator end() const { return iterator(end_); }

    size_type size() {
        return cont_.size();
    }

private:
    void DoStaticAsserts() {
        static_assert(ForwardIterator<Iterator>, "Iterator should be at least forward!");
        static_assert(IsAssociative<std::remove_reference_t<Container>>, "Not an associative container");
    }

    Container& cont_;
    Iterator begin_;
    Iterator end_;
};
}
