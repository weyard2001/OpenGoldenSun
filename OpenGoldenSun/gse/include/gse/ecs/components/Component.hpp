#pragma once

#include <cstdint>
#include <cassert>
#include <bitset>

constexpr std::uint8_t MAX_COMPONENTS = 32; // max number of components and max for each entity

namespace gse {
    using ComponentMask = std::bitset<MAX_COMPONENTS>;

    struct IComponent {
    public:
        using ID = std::uint32_t;

    protected:
        static ID m_id;
    };

    template <typename T>
    class Component : public IComponent {
    public:
        static ID& getID() {
            assert(m_id < MAX_COMPONENTS);
            static auto componentID = m_id++;

            return componentID;
        }
    };
}