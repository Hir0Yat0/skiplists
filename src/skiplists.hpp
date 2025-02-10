#ifndef SKIPLISTS_HPP
#define SKIPLISTS_HPP

#include <cstddef>
#include "node.hpp"
#include "nodemanager.hpp"

namespace skiplists {




class SkipLists final {

    public:

    SkipLists()
    : nodeManager{}
    {};

    /*
        returns True if insert complete, else false
    */
    constexpr bool insertIfNotExist (const std::size_t key, const int val) noexcept {
        return nodeManager.insertIfNotExist(key,val);
    };

    constexpr void insertOrReplace(const std::size_t key, const int val) noexcept {
        nodeManager.insertOrReplace(key, val);
    }

    /*
        returns True if successfully remove element
    */
    constexpr bool remove(const std::size_t key) noexcept {
        return nodeManager.remove(key);
    }
    [[nodiscard]]
    constexpr bool contains(const std::size_t key) const noexcept {
        return nodeManager.contains(key);
    }
    [[nodiscard]]
    constexpr std::size_t size(void) const noexcept {
        return nodeManager.size();
    }

    constexpr void printSkipLists(void) const noexcept {
        nodeManager.printSkipLists();
    }

    private:

    NodeManager nodeManager;

};

} // namespace skiplists


#endif // SKIPLISTS_HPP
