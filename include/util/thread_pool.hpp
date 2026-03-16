#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace util {

class thread_pool {
public:
    explicit thread_pool(size_t thread_count) {
        if (thread_count == 0) {
            thread_count = 1;
        }

        workers.reserve(thread_count);
        for (size_t i = 0; i < thread_count; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this]() -> bool {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ~thread_pool() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();

        for (std::thread &worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    template <typename Function, typename... Args>
    auto enqueue(Function &&function, Args &&...args)
        -> std::future<std::invoke_result_t<Function, Args...>> {
        using result_type = std::invoke_result_t<Function, Args...>;

        auto task = std::make_shared<std::packaged_task<result_type()>>(
            std::bind(std::forward<Function>(function), std::forward<Args>(args)...)
        );

        std::future<result_type> future = task->get_future();
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            tasks.emplace([task]() {
                (*task)();
            });
        }
        condition.notify_one();
        return future;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;
};

}

#endif