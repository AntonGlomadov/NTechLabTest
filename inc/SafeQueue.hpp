#pragma once
#include <thread>
#include <memory>
#include <mutex>
#include <queue>
#include <type_traits>
#include <cstdint>
namespace threads::safe {
    template<typename T, size_t limit = SIZE_MAX>
/**
  * Потоко безопасная очередь
  * 
  *
  */
    class Queue {
    public:
/**
  * Функция добавления в очередь
  * 
  * @param t - ссылка на обьект
  * @return - возвращает true/false - в зависимости от успешности операции
  *
  */
        bool Push(const T &t) {
            std::lock_guard lock(m_m);
            if (m_queue.size() == limit)
                return false;

            m_queue.push(t);
            return true;
        }
/**
  * Функция добавления в очередь
  * 
  * @param t - rvalue или lvalue обьект
  * @return - возвращает true/false - в зависимости от успешности операции
  *
  */
        bool Push(T &&t) {
            std::lock_guard lock(m_m);
            if (m_queue.size() == limit)
                return false;
            m_queue.push(std::forward<T>(t));//for perfect forwarding
            return true;
        }
        /**
  * Функция извлечения обьекта из очереди
  * 
  * @return - возвращает ссылку на обьект
  *
  */
        std::shared_ptr<T> Extract() {
            std::lock_guard lock(m_m);
            if (m_queue.empty())
                return nullptr;

            std::shared_ptr<T> val = nullptr;
            if constexpr (std::is_move_constructible_v<T>)
                val = std::make_shared<T>(std::move(m_queue.front()));
            else
                val = std::make_shared<T>(m_queue.front());
            m_queue.pop();
            return val;
        }
/**
  * Функция для проверки пустая ли очередь
  * 
  * @return - true/false
  *
  */
        bool Empty() const {
            std::lock_guard lock(m_m);
            return m_queue.empty();
        }
/**
  * Функция для получения текущего кол-ва элементов в очереди
  * 
  * @return - size_t
  *
  */
        size_t Size() const {
            std::lock_guard lock(m_m);
            return m_queue.size();
        }
/**
  * Функция для получения максимального размера очереди
  * 
  * @return - size_t
  *
  */
        constexpr size_t MaxSize() {
            return limit;
        }

    private:
        mutable std::mutex m_m;
        std::queue<T> m_queue;
    };
}