#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <climits>
#include "task_des.h"
//C++ queue in Standard Library is not thread safe
//did not try to use the template
class ThreadSafeQueue {
private:
    std::queue<task_des> data;
    mutable std::mutex mut;
    std::condition_variable data_cond;

public:
    ThreadSafeQueue() {}
    ThreadSafeQueue(const ThreadSafeQueue& d) {
        this->data = data;
    }
    void push(const task_des val) {
        std::lock_guard<std::mutex> lk(mut);
        data.push(val);
        data_cond.notify_one();
    }

    void wait_and_pop(task_des& val) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {return !data.empty(); });
        val = data.front();
        data.pop();
    }

    std::shared_ptr<task_des> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {return !data.empty(); });
        std::shared_ptr<task_des> res(
            std::make_shared<task_des>(data.front()));
        data.pop();
        return res;
    }

    bool try_pop(task_des& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data.empty())
            return false;
        value = data.front();
        data.pop();
    }

    std::shared_ptr<task_des> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data.empty())
            return {};
        std::shared_ptr<task_des> res(
            std::make_shared<task_des>(data.front()));
        data.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data.empty();
    }
};