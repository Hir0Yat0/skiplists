#ifndef NODEWALKER_HPP
#define NODEWALKER_HPP

#include <cstddef>
#include <memory>
#include <vector>
#include <optional>
#include "node.hpp"
#include "sentinelnodemanager.hpp"
#include "logger.hpp"

namespace skiplists {
    
struct NodeWalkerResults final {
    
    // last item in vector points to target nodes (if found) (i.e. first item in vector is top level of stack)
    std::vector<std::shared_ptr<Node>> nodesOnLeftBeforeMovingDown;
    bool hasFoundTarget;

    constexpr NodeWalkerResults(const std::size_t maxHeight): nodesOnLeftBeforeMovingDown(){
        nodesOnLeftBeforeMovingDown.reserve(maxHeight);
    }
    [[nodiscard]]
    constexpr std::optional<std::shared_ptr<Node>> getNodePointingToTargetsBaseNode(void) const noexcept {
        if (hasFoundTarget){
            return std::optional<std::shared_ptr<Node>>(nodesOnLeftBeforeMovingDown.at(nodesOnLeftBeforeMovingDown.size()-1));
        }
        else {
            return std::nullopt;
        }
    }

};


class NodeWalker final {

    public:
    [[gnu::hot, nodiscard]]
    constexpr std::unique_ptr<NodeWalkerResults> walkUntilFindNodePointingToTarget(const SentinelNodeManager & sentinelNodeManager, const std::size_t targetKey) const noexcept {

        std::unique_ptr<NodeWalkerResults> nodeWalkerResults = std::make_unique<NodeWalkerResults>(sentinelNodeManager.getSentinelCurrentMaxStackHeight());

        (*nodeWalkerResults).hasFoundTarget = false;

        std::shared_ptr<Node> currentNode = sentinelNodeManager.getTopMostSentinelNode();
        while (currentNode){
            if (!isOverShoot((*currentNode), targetKey)){
                if (isTarget((*currentNode),targetKey)) { // TODO: MOVE to !Overshoot?
                    DBGLOG("target found!");
                    (*nodeWalkerResults).hasFoundTarget = true; 
                    recordTheRestOfTowerDownwards((*nodeWalkerResults),(*currentNode));
                    break;
                }
                else {
                    moveRight(currentNode); 
                    continue;

                }
            }
            else {
                (*nodeWalkerResults).nodesOnLeftBeforeMovingDown.push_back(currentNode);
                moveDown(currentNode);
            }
        }

        return nodeWalkerResults;

    }

    private:

    [[gnu::hot, nodiscard]]
    constexpr bool isTarget(const Node & currentNode, const std::size_t targetKey) const noexcept {
        return currentNode.rightNode && (*currentNode.rightNode).key == targetKey;
    }
    [[gnu::hot, nodiscard]]
    constexpr bool isOverShoot(const Node & currentNode, const std::size_t targetKey) const noexcept {
        return !currentNode.rightNode || (*currentNode.rightNode).key > targetKey;
    }
    [[gnu::hot]]
    constexpr void moveDown(std::shared_ptr<Node> & currentNode) const noexcept {
        currentNode = (*currentNode).downNode;
    }
    [[gnu::hot]]
    constexpr void moveRight(std::shared_ptr<Node> & currentNode) const noexcept {
        currentNode = (*currentNode).rightNode;
    }
    constexpr void recordTheRestOfTowerDownwards(NodeWalkerResults & nodeWalkerResults, const Node & currentNode) const noexcept {
        std::shared_ptr<Node> nextNode = currentNode.downNode;
        while (nextNode){
            nodeWalkerResults.nodesOnLeftBeforeMovingDown.push_back(nextNode);
            moveDown(nextNode);
        }
    } 
};


} // namespace skiplists

#endif // NODEWALKER_HPP
