#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

namespace skiplists {
#if 0
class Logger {

    public:

    constexpr static log() {

    }


};

#endif

#define DEBUG_PRINT_MODE_ON 1

#define DBGEXPR(EXPR) { \
    if constexpr (DEBUG_PRINT_MODE_ON) { \
        std::cerr << #EXPR << ": " << EXPR << std::endl; \
    }  \
};

#define DBGLOG(MSG) { \
    if constexpr(DEBUG_PRINT_MODE_ON) { \
        std::cerr << MSG << std::endl; \
    } \
};

} // namespace skiplists

#endif // LOGGER_HPP
