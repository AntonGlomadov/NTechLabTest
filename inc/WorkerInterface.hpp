#pragma once

#include <future>
#include "Matrix.hpp"
#include "ThreadPool.hpp"

class WorkerInterface {
public:
    WorkerInterface() = default;

    WorkerInterface(WorkerInterface&&) = delete;
    WorkerInterface(const WorkerInterface&) = delete;

    WorkerInterface& operator=(WorkerInterface&&) = delete;
    WorkerInterface& operator=(const WorkerInterface&) = delete;

    virtual ~WorkerInterface() = default;

    virtual std::future<Matrix> AsyncProcess(Matrix) = 0;
};
/**
 * Реализация WorkerInterface которая выполняет транспонирование матрицы
*/
class WorkerTranspose : public WorkerInterface {
public:
    std::future<Matrix> AsyncProcess(Matrix matrix) override {
        std::future<Matrix> res = thread_pool.Push(matrix,TransposeMatrix);
        return res;
    }

    explicit WorkerTranspose(threads::ThreadPool& thread_pool) : thread_pool(thread_pool) {}
private:
    threads::ThreadPool& thread_pool;
};

/*
*Фабрика воркеров
*/
class WorkerFactory {
public:
    static std::shared_ptr<WorkerInterface> CreateWorker() {
        return std::make_shared<WorkerTranspose>(thread_pool);
    }

private:
    static  constexpr size_t thread_pool_size = 4;
    static inline threads::ThreadPool thread_pool{thread_pool_size};
};

std::shared_ptr<WorkerInterface> get_new_worker() {
    return WorkerFactory::CreateWorker();
}
