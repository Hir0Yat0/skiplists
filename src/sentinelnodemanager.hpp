#ifndef SENTINELNODEMANAGER_HPP
#define SENTINELNODEMANAGER_HPP

#include <cstddef>
#include <memory>
#include <vector>
#include "node.hpp"
#include "logger.hpp"

namespace skiplists {
    
#define ENABLE_SOFT_LIMIT_SHRINKING_SIZE 0

class SentinelNodeManager final {

    public:

    constexpr SentinelNodeManager(
        const std::size_t initialHeight = defaultInitialSentinelHeight
    )
    :
        sentinels{} 
    {
        // initSentinelStack(initialHeight);
        initSentinelStack(initialHeight);
    }
    [[nodiscard]]
    constexpr std::shared_ptr<skiplists::Node> getFirstTopNode(void) const noexcept {
        return (*this->getTopMostSentinelNode()).rightNode;
    }
    [[nodiscard]]
    constexpr std::shared_ptr<skiplists::Node> getTopMostSentinelNode(void) const noexcept {
        return this->sentinels.at(this->sentinels.size() - 1);
    }
    [[nodiscard]]
    constexpr std::size_t getSentinelCurrentMaxStackHeight(void) const noexcept {
        return this->sentinels.size();
    }

    constexpr void growSentinelTower(const std::size_t byLevelAmount) noexcept {
        if (byLevelAmount > 0){
            sentinels.push_back(newSentinel());
            for (std::size_t i = 1; i < byLevelAmount; i++){
                sentinels.push_back(newSentinel());
                (*sentinels[i]).downNode = sentinels[i-1];
            }

        }
    }

    constexpr void shrinkSentinelTower(const std::size_t byLevelAmount) noexcept {
        # if ENABLE_SOFT_LIMIT_SHRINKING_SIZE
        if (sentinels.size() > defaultInitialSentinelHeight){
            if (byLevelAmount + defaultInitialSentinelHeight <= sentinels.size()){
                for (std::size_t i = 0; i < byLevelAmount; i++){
                    sentinels.pop_back();
                }
            }
            else {
                shrinkSentinelTowerToUnchecked(defaultInitialSentinelHeight);
            }
        }
        #else
        DBGLOG("sent size");
        DBGLOG(sentinels.size());
        DBGLOG("shrink amnt");
        DBGLOG(byLevelAmount);
        if (sentinels.size() > 0){
            if (sentinels.size() > byLevelAmount) {
                for (std::size_t i = 0; i < byLevelAmount; i++){
                    sentinels.pop_back();
                }        
            }
            else {
                shrinkSentinelTowerToUnchecked(0);
            }
        }
        #endif

    }

    constexpr const std::vector<std::shared_ptr<skiplists::Node>> & getSentinelsReadOnlys(void) const noexcept { 
        return sentinels;
    }

    private:

    constexpr static std::size_t defaultInitialSentinelHeight = 1;

    std::vector<std::shared_ptr<skiplists::Node>> sentinels;

    constexpr std::shared_ptr<skiplists::Node> newSentinel(void) const noexcept {
        return std::make_shared<skiplists::Node>(0,0,nullptr,nullptr);
    }

    [[gnu::cold]]
    constexpr void initSentinelStack(const std::size_t initialHeight) noexcept {
        sentinels.reserve(initialHeight);
        growSentinelTower(initialHeight);
        
    }

    constexpr void shrinkSentinelTowerToUnchecked(const std::size_t toLevelAmount) noexcept {
        while (sentinels.size() > toLevelAmount){
            sentinels.pop_back();
        }
    }

};

} // namespace skiplists


#endif // SENTINELNODEMANAGER_HPP
