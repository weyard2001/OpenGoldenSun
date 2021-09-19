#pragma once

#include <vector>
#include <iostream>

namespace gse {
    class IPool {
    public:
        IPool() = default;
        virtual ~IPool() = default;
        virtual void removeEntity(std::uint32_t entityID) = 0;
    };

    template <class T>
    class ComponentPool final : public IPool {
    public:
        ComponentPool(std::uint32_t size = 128)
            : m_pool(size) {

        }

        virtual ~ComponentPool() = default;

        // TODO: check if entity has such component. currently, it just checks if its ID is within pool size and resets the component data.
        void removeEntity(std::uint32_t entityID) override {
            if (entityID < m_pool.size()) {
                m_pool[entityID] = T();
            }
        }

        void add(T object) {
            m_pool.push_back(object);
        }

        std::uint32_t size() const {
            return m_pool.size();
        }

        bool empty() const {
            return m_pool.empty();
        }

        void clear() {
            m_pool.clear();
        }

        void resize(std::uint32_t size) {
            m_pool.resize(size);
        }

        T& operator[](std::uint32_t index) {
            return m_pool[index];
        }

    private:
        std::vector<T> m_pool;
    };
}