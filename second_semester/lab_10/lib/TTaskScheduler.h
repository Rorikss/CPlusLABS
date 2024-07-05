//
// Created by Roriks on 4/10/2024.
//
#pragma once
#include <vector>
#include <algorithm>
#include <tuple>
#include "Any.h"

namespace Scheduler {
class TTaskScheduler {
private:
    class TaskBase {
    public:
        virtual ~TaskBase() = default;
        virtual void invoke() = 0;
        virtual bool IsExecuted() = 0;
        virtual Any GetResult() = 0;
        virtual size_t GetId() = 0;
    };

    template <typename T>
    class FutureResult {
    public:
        FutureResult(std::shared_ptr<TaskBase> ptr) : ptr_(ptr) {}

        operator T() {
            return  ptr_->GetResult().Cast<T>();
        }

    private:
        std::shared_ptr<TaskBase> ptr_;
    };

    template <typename Func, typename... Args>
    class Task : public TaskBase {
    public:
        Task(Func function, size_t id, Args... args)
                : function_(function)
                , args_(std::make_tuple(args...))
                , id_(id)
                , is_executed_(false)
                {}

        void invoke() override {
            result_ = std::apply(function_, args_);
        }

        bool IsExecuted() override {
            return is_executed_;
        }

        Any GetResult() override {
            return result_;
        }

        size_t GetId() override {
            return id_;
        }

        std::tuple<Args...> args_;
        Func function_;
        size_t id_;
        Any result_;
        bool is_executed_;
    };

public:
    template <typename Func, typename... Args>
    auto add(Func func, Args... args) {
        auto task = std::make_shared<Task<Func, Args...>>(func, next_id_++, args...);
        return AddHelper(task);
    }

    template <typename T>
    FutureResult<T> getFutureResult(size_t id) {
        dependency_graph_[id].push_back(next_id_);
        is_top_sorted_ = false;
        return FutureResult<T>(tasks_[id]);
    }

    template <typename T>
    T getResult(size_t id) {
        auto task = tasks_[id];
        if (!task->IsExecuted()) {
            executeAll();
        }

        return task->GetResult().Cast<T>();
    }

    void executeAll() {
        if (!is_top_sorted_) {
            TopSort();
        }
        for (int i = top_sort_order_.size() - 1; i >= 0; --i) {
            if (!tasks_[top_sort_order_[i]]->IsExecuted()) {
                tasks_[top_sort_order_[i]]->invoke();
            }
        }
    }

private:
    void DFS(size_t vertex, std::vector<bool>& visited) {
        visited[vertex] = true;
        for (auto u: dependency_graph_[vertex]) {
            if (!visited[u])
                DFS(u, visited);
        }
        top_sort_order_.push_back(vertex);
    }

    void TopSort() {
        std::vector<bool> visited(next_id_);
        top_sort_order_.clear();
        for (size_t i = 0; i < next_id_; ++i) {
            if (!visited[i]) {
                DFS(i, visited);
            }
        }
        is_top_sorted_ = true;
    }

    size_t AddHelper(std::shared_ptr<TaskBase> task) {
        dependency_graph_.push_back(std::vector<size_t>{});
        top_sort_order_.push_back(next_id_ - 1);
        tasks_.push_back(task);
        return (next_id_ - 1);
    }

    std::vector<std::shared_ptr<TaskBase>> tasks_;
    std::vector<std::vector<size_t>> dependency_graph_;
    std::vector<size_t> top_sort_order_;
    size_t next_id_ = 0;
    bool is_top_sorted_ = false;
};
} // end of namespace

