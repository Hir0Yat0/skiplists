#ifndef COINFLIPPER_HPP
#define COINFLIPPER_HPP

#include <cstddef>
#include <cstdlib>
#include <ctime>

namespace skiplists {
    
enum class CoinSide {
    HEAD = 0,
    TAIL = 1,
    SIDE,
    UNKNOWN,
};

class CoinFlipper {

    public:

    CoinFlipper() {
        // randomize seeds with times
        std::srand(time(0));    
    }
    [[nodiscard]]
    constexpr std::size_t flipCoinAndGetNumConsecutiveTargetSide(void) const noexcept {
        std::size_t counts = 0;
        while (flipACoin() == CoinSide::HEAD){
            counts++;
        }
        // return counts;
        return 5;
    }
    [[nodiscard]]
    constexpr CoinSide flipACoin(void) const noexcept {
        return static_cast<CoinSide>(std::rand() % 2);
    }

    private:

};

} // namespace skiplists


#endif // COINFLIPPER_HPP
