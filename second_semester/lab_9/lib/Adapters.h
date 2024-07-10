//
// Created by Roriks on 3/20/2024.
//
#pragma once
#include <iostream>
#include <iterator>
#include "Iterators.h"
#include "Ranges.h"

namespace adapters {

// functors
// 1. transform
template<typename UnaryFunction>
class transform {
public:
    transform(UnaryFunction func) : func_(func) {}

    template<typename Range>
    auto MakeAdapter(Range&& range) {
        return RangeContainer<decltype(std::begin(range)),
                TransformIterator<decltype(std::begin(range)), UnaryFunction>, Range, UnaryFunction>(
                std::begin(range), std::end(range), range, func_);
    }

private:
    UnaryFunction func_;
};

// 2. filter
template<typename Predicate>
class filter {
public:
    filter(Predicate func) : func_(func) {
        static_assert(std::is_convertible<Predicate, bool>::value, "Predicate must be convertible to bool");
    }

    template<typename Range>
    auto MakeAdapter(Range&& range) {
        return RangeContainer<decltype(std::begin(range)),
                FilterIterator<decltype(std::begin(range)), Predicate>, Range, Predicate>(
                std::begin(range), std::end(range), range, func_);
    }

private:
    Predicate func_;
};

// 3. take
class take {
public:
    take(size_t n) : skipping_number_(n) {}

    template<typename Range>
    auto MakeAdapter(Range&& range) {
        auto offset = std::min(range.size(), skipping_number_);
        return RangeContainer<decltype(std::begin(range)),
                TakeAndDropIterator<decltype(std::begin(range))>, Range>(
                std::begin(range), std::next(std::begin(range), offset), range);
    }

private:
    size_t skipping_number_;
};

// 4. drop
class drop {
public:
    drop(size_t n) : skipping_number_(n) {}

    template<typename Range>
    auto MakeAdapter(Range&& range) {
        auto offset = std::min(range.size(), skipping_number_);
        return RangeContainer<decltype(std::begin(range)),
                TakeAndDropIterator<decltype(std::begin(range))>, Range>(
                std::next(std::begin(range), offset), std::end(range), range);
    }

private:
    size_t skipping_number_;
};

// 5. reverse
class reverse {
public:
    reverse() = default;

    template<typename Range>
    auto MakeAdapter(Range&& range) {
        static_assert(ReversibleIterator<decltype(std::begin(range))>, "Not reversible container!"); // todo make concept!!
        return RangeContainer<decltype(std::begin(range)),
                ReverseIterator<decltype(std::begin(range))>, Range>(
                std::end(range), std::begin(range), range);
    }
};

// 6. keys !!! for associative implementation
class keys {
public:
    keys() = default;

    template<typename Range>
    auto MakeAdapter(Range&& range) {
        return AssociativeRange<decltype(std::begin(range)), Range, keys>
                                            (range, std::begin(range), std::end(range));
    }
};

// 7. values !!! for associative implementation
class values {
public:
    values() = default;

    template<typename Range>
    auto MakeAdapter(Range&& range) {
        return AssociativeRange<decltype(std::begin(range)), Range, values>
                                                (range, std::begin(range), std::end(range));
    }
};

// overload of pipe operator
template <typename Container, typename Functor>
auto operator|(Container&& container, Functor func) {
    return func.MakeAdapter(container);
}

} // end of namespace
