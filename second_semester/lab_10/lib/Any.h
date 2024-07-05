//
// Created by Roriks on 4/11/2024.
//
#pragma once

#include <iostream>
#include <memory>
#include <typeinfo>
#include <type_traits>

namespace Scheduler {

class Any {
public:
    Any() : content_(nullptr) {}

    Any(const Any& other)
            : content_(other.content_ ? other.content_->MakeCopy() : nullptr)
            {}

    Any& operator=(const Any& other) {
        Any(other).Swap(*this);
        return *this;
    }

    Any(Any&& other) noexcept
            : content_(std::move(other.content_))
            {}


    Any& operator=(Any&& other) noexcept {
        Any(std::move(other)).Swap(*this);
        return *this;
    }

    template<typename T>
    Any& operator=(const T& value) {
        content_ = std::make_unique<Holder<T>>(value);
        return *this;
    }

    template<typename T>
    T& Cast() {
        if (typeid(T) != content_->GetType()) {
            throw BadAnyCast("Task has been created with another type!");
        }
        return static_cast<Holder<T>*>(content_.get())->value_;
    }

    template<typename T>
    const T& Cast() const {
        if (typeid(T) != content_->GetType()) {
            throw BadAnyCast("Task has been created with another type!");
        }
        return static_cast<Holder<T>*>(content_.get())->value_;
    }

    void Swap(Any& other) {
        std::swap(content_, other.content_);
    }

private:
    class PlaceHolder {
    public:
        virtual ~PlaceHolder() = default;

        virtual const std::type_info& GetType() const = 0;

        virtual PlaceHolder* MakeCopy() const = 0;
    };

    template<typename T>
    class Holder : public PlaceHolder {
    public:
        Holder(T value) : value_(value) {}

        const std::type_info& GetType() const override {
            return typeid(T);
        }

        PlaceHolder* MakeCopy() const override {
            return new Holder(value_);
        }

        T value_;
    };

    class BadAnyCast : public std::exception {
    public:
        BadAnyCast(const char* msg) : message(msg) {}

        const char* what() const throw() {
            return message.c_str();
        }

    private:
        std::string message;
    };

    std::unique_ptr<PlaceHolder> content_;
};
} // end of namespace

