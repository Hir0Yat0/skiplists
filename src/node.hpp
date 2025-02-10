#ifndef NODE_HPP
#define NODE_HPP

#include <cstddef>
#include <memory>
#include <iostream>

namespace skiplists {
    
#define EXTRA_NODE_INFO 1

class Node final {

    public:

    std::size_t key;
    int val;

    std::shared_ptr<Node> rightNode;
    std::shared_ptr<Node> downNode;

#if EXTRA_NODE_INFO
    std::size_t stackHeight;
    std::size_t currentHeight;
#endif

    Node(const std::size_t key, const int val, const std::shared_ptr<Node> & rightNode = nullptr, const std::shared_ptr<Node> & downNode = nullptr) noexcept
    : key{key}, val{val}, rightNode{rightNode}, downNode{downNode}
#if EXTRA_NODE_INFO
        , stackHeight{}, currentHeight{}
#endif
    {}

    constexpr void printNode(void) const noexcept {
        std::cout << "(" << key << ", " << val << ", "
#if EXTRA_NODE_INFO
            << stackHeight << ", "
            << currentHeight << ", "
#endif
            << ")";
    }

    private:

};

} // namespace skiplists


#endif // NODE_HPP
