#ifndef NODEMANAGER_HPP
#define NODEMANAGER_HPP

#include <cstddef>
#include <memory>
#include <cassert>
#include "node.hpp"
#include "sentinelnodemanager.hpp"
#include "nodewalker.hpp"
#include "coinflipper.hpp"
#include "logger.hpp"

namespace skiplists {

#define ENABLE_NODE_WALK_RESULT_CACHE 0

#if ENABLE_NODE_WALK_RESULT_CACHE

struct NodeWalkResultCache {

    std::unique_ptr<NodeWalkerResults> nodeWalkerResults;
    std::size_t key;
    bool valid;
    NodeWalkResultCache(): nodeWalkerResults{nullptr}, key{0}, valid{false} {

    }

};

#endif

class NodeManager final {

    public:

    NodeManager(

    )
    :
        sentinelNodeManager{},
        nodeWalker{},
        coinFlipper{},
        mSize{0}
    {

    }

    /*
        returns True if insert complete, else false
    */
    constexpr bool insertIfNotExist (const std::size_t key, const int val) noexcept {
        const std::size_t newNodeHeight = getNewHeight();
        growSentinelsHeightsIfNotEnough(newNodeHeight);
        const std::unique_ptr<NodeWalkerResults> nodeWalkerResults = walkUntilFindNodePointingToTarget(key);
        if (!(*nodeWalkerResults).hasFoundTarget){
            insert(key,val,(*nodeWalkerResults),newNodeHeight);
            shrinkHeightsIfLevelsAreEmpty();
            mSize++;
            return true;
        }
        shrinkHeightsIfLevelsAreEmpty();
        return false;
    };

    constexpr void insertOrReplace(const std::size_t key, const int val) noexcept {
        const std::size_t newNodeHeight = getNewHeight();
        growSentinelsHeightsIfNotEnough(newNodeHeight);
        const std::unique_ptr<NodeWalkerResults> nodeWalkerResults = walkUntilFindNodePointingToTarget(key);
        if (!(*nodeWalkerResults).hasFoundTarget){
            insert(key,val,(*nodeWalkerResults),newNodeHeight);
            shrinkHeightsIfLevelsAreEmpty();
            mSize++;
        }
        else {
            replace(key,val,(*nodeWalkerResults));
            shrinkHeightsIfLevelsAreEmpty();
        }
    }

    /*
        returns True if successfully remove element
    */
    constexpr bool remove(const std::size_t key) noexcept {
        const std::unique_ptr<NodeWalkerResults> nodeWalkerResults = walkUntilFindNodePointingToTarget(key);
        if (!(*nodeWalkerResults).hasFoundTarget){
            shrinkHeightsIfLevelsAreEmpty();
            return false;
        }
        else {
            removeNodes(key,(*nodeWalkerResults));
            shrinkHeightsIfLevelsAreEmpty();
            mSize--;
            return true;
        }
    }
    [[nodiscard]]
    constexpr bool contains(const std::size_t key) const noexcept {
        return (*walkUntilFindNodePointingToTarget(key)).hasFoundTarget;
    }
    [[nodiscard]]
    constexpr std::size_t size(void) const noexcept {
        return mSize;
    }

    constexpr void printSkipLists(void) const noexcept {
        for (const auto & currentSentinel : sentinelNodeManager.getSentinelsReadOnlys()){
            auto & currentNode = (*currentSentinel).rightNode;
            std::cout << "sen" << "   ";
            while (currentNode){
                (*currentNode).printNode();
                currentNode = (*currentNode).rightNode;
            }
            std::cout << "\n";
        }
    }

    private:

    constexpr static std::size_t numSpaceBetweenNodesWhenPrinting = 2;

    SentinelNodeManager sentinelNodeManager;
    NodeWalker nodeWalker;
    CoinFlipper coinFlipper;
    std::size_t mSize;

    [[nodiscard]]
    constexpr std::unique_ptr<NodeWalkerResults> walkUntilFindNodePointingToTarget(const std::size_t targetKey) const noexcept {
        return nodeWalker.walkUntilFindNodePointingToTarget(sentinelNodeManager,targetKey);
    }

    constexpr void insert(const std::size_t key, const int val, const NodeWalkerResults & nodeWalkerResults, const std::size_t newNodeHeight)  {
        {
            // insert first base level first
            // DBGLOG("1");
            auto & nodeOnBaseLevelPointingToTargetBaseNode = nodeWalkerResults.nodesOnLeftBeforeMovingDown[nodeWalkerResults.nodesOnLeftBeforeMovingDown.size() - 1];
            // DBGLOG("1.5");
            (*nodeOnBaseLevelPointingToTargetBaseNode).printNode(); // possibliy sentinel
            // DBGLOG("2");
            std::shared_ptr<Node> newNode = std::make_shared<Node>(key,val,(*nodeOnBaseLevelPointingToTargetBaseNode).rightNode,nullptr);
            // if ((*nodeOnBaseLevelPointingToTargetBaseNode).rightNode){
            //     (*newNode).rightNode = (*(*nodeOnBaseLevelPointingToTargetBaseNode).rightNode).rightNode;
            // }
            # if EXTRA_NODE_INFO
            (*newNode).stackHeight = newNodeHeight;
            (*newNode).currentHeight = 0;
            # endif
            // DBGLOG("3");
            (*nodeOnBaseLevelPointingToTargetBaseNode).rightNode = newNode;
            // DBGLOG("4");
        }
        for (std::size_t currentLevel = 1; currentLevel < newNodeHeight; currentLevel++){
            // walk from bottom up  
            auto & nodeOnCurrentLevelPointingToTarget = nodeWalkerResults.nodesOnLeftBeforeMovingDown[nodeWalkerResults.nodesOnLeftBeforeMovingDown.size()  - currentLevel];
            if (!nodeOnCurrentLevelPointingToTarget){
                DBGLOG("sen null");
                DBGEXPR(nodeOnCurrentLevelPointingToTarget);
            }
            else {
                DBGLOG("sen not null");
                DBGEXPR(nodeOnCurrentLevelPointingToTarget);
            }
            if (!(*nodeOnCurrentLevelPointingToTarget).rightNode){
                DBGLOG("sen right null");
                DBGEXPR((*nodeOnCurrentLevelPointingToTarget).rightNode);
            }
            else {
                DBGLOG("sen right not null");
                DBGEXPR((*nodeOnCurrentLevelPointingToTarget).rightNode);
            }
            std::shared_ptr<Node> newNode = std::make_shared<Node>(key,val,(*nodeOnCurrentLevelPointingToTarget).rightNode,nullptr);
            // if ((*nodeOnCurrentLevelPointingToTarget).rightNode){
            //     (*newNode).rightNode = (*(*nodeOnCurrentLevelPointingToTarget).rightNode).rightNode;
            // }
            (*nodeOnCurrentLevelPointingToTarget).rightNode = newNode;
            # if EXTRA_NODE_INFO
            (*newNode).stackHeight = newNodeHeight;
            (*newNode).currentHeight = currentLevel;
            # endif
            // assign down node to prev's pointer-to-target's right
            const auto & nodeOnPrevLevelPointingToBelowTarget = nodeWalkerResults.nodesOnLeftBeforeMovingDown[nodeWalkerResults.nodesOnLeftBeforeMovingDown.size() - currentLevel];
            (*newNode).downNode = (*nodeOnPrevLevelPointingToBelowTarget).rightNode;
        }
    }

    constexpr void replace(const std::size_t key, const int val, const NodeWalkerResults & nodeWalkerResults) noexcept {
        for (const auto & currentNodePointingToTarget : nodeWalkerResults.nodesOnLeftBeforeMovingDown){
            auto & targetNode = (*currentNodePointingToTarget).rightNode;
            if (targetNode && (*targetNode).key == key){
                (*targetNode).val = val;
            }
        }
    }

    constexpr void removeNodes(const std::size_t key, const NodeWalkerResults & nodeWalkerResults) noexcept {
        for (const auto & currentNodePointingToTarget : nodeWalkerResults.nodesOnLeftBeforeMovingDown){
            auto & targetNode = (*currentNodePointingToTarget).rightNode;
            if (targetNode){
                if ((*targetNode).key == key){
                    (*currentNodePointingToTarget).rightNode = (*targetNode).rightNode;
                }
            }
        }
    }

    [[nodiscard]]
    constexpr std::size_t getNewHeight(void) const noexcept {
        return coinFlipper.flipCoinAndGetNumConsecutiveTargetSide() + 1; // add one cuz no consecutive = 0 counts = 1 levels
    }

    constexpr void shrinkHeightsIfLevelsAreEmpty(void) noexcept {
        const std::size_t emptyLevelsCounts = countEmptyLevels();
        sentinelNodeManager.shrinkSentinelTower(emptyLevelsCounts);
    }

    constexpr void growSentinelsHeightsIfNotEnough(const std::size_t newNodeHeight) noexcept {
        if (newNodeHeight > sentinelNodeManager.getSentinelCurrentMaxStackHeight()){
            sentinelNodeManager.growSentinelTower(newNodeHeight - sentinelNodeManager.getSentinelCurrentMaxStackHeight());
        }
    }

    constexpr bool levelIsEmpty(const Node & currentSentinel) const noexcept {
        return !currentSentinel.rightNode;
    }

    constexpr std::size_t countEmptyLevels() const noexcept {
        std::size_t emptyLevelsCounts = 0;
        for (const auto & currentSentinel : sentinelNodeManager.getSentinelsReadOnlys()){
            if (levelIsEmpty((*currentSentinel))){
                emptyLevelsCounts++;
            }
            else {
                // break;                                                                                      
            }
        }
        return emptyLevelsCounts;
    }

};

} // namespace skiplists


#endif // NODEMANAGER_HPP
