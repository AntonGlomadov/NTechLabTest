#pragma once


#include <vector>
#include <future>
#include <queue>
#include <functional>
#include <mutex>
#include <utility>
#include <thread>

#include "Matrix.hpp"
#include "SafeQueue.hpp"

namespace threads {

/**
  * Пулпотоков
  * 
  *
  */

    class ThreadPool {
    public:
        explicit ThreadPool(size_t thread_pool_size);

        std::future<Matrix> Push(Matrix &matrix,std::function<Matrix(Matrix)>);

        ~ThreadPool() noexcept;

        ThreadPool(const ThreadPool &) = delete;

        ThreadPool(ThreadPool &&) = delete;


    private:
        safe::Queue<std::packaged_task<Matrix()>> m_tasks;
        std::vector<std::thread> m_pool;
        size_t m_count;

        std::atomic_bool m_stop = false;
        std::condition_variable m_cv;
    };
/**
  * Функция добавления задачи в очередь.
  * 
  * @param matrix - входная матрицы для транспонирования.
  * @param func - операция над матрицейъ
  * @return std::future<Matrix> - результат операции над матрицей
  *
  */
    inline std::future<Matrix> ThreadPool::Push(Matrix &matrix,std::function<Matrix(Matrix)>func) {
        auto task = std::packaged_task <Matrix() > (std::bind( func,matrix));
        auto result = task.get_future();
        m_tasks.Push(std::move(task));
        m_cv.notify_one();
        return result;
    }

/**
  * Конструктор
  * 
  * @param thread_pool_size - кол-во потоков в в thread pool
  *
  */
    inline ThreadPool::ThreadPool(size_t thread_pool_size) {
        if (thread_pool_size>0){
            m_count = thread_pool_size;
        }

        for (size_t i = 0; i < m_count; i++) {
            m_pool.emplace_back([&] {
                while (!m_stop) {
                    {
                        auto taskPtr = m_tasks.Extract();
                        if(taskPtr == nullptr) {
                            std::mutex m;
                            std::unique_lock lock(m);
                            m_cv.wait(lock);
                            continue;
                        }
                        taskPtr->operator()();
                    }
                }
            });
        }
    }

    inline ThreadPool::~ThreadPool() {
        m_stop = true;
        m_cv.notify_all();
        for (auto &t: m_pool) {
            t.join();
        }
    }
}