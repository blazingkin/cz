#include <cz/assert.hpp>

#include <stdlib.h>

#ifdef TRACY_ENABLE
#include <stdio.h>
#include <Tracy.hpp>
#include <algorithm>
#endif

namespace cz {

PanicReachedException::PanicReachedException(const char* message) : message(message) {}

const char* PanicReachedException::what() const noexcept {
    return message;
}

namespace impl {

void panic_reached(const char* message) {
#ifdef TRACY_ENABLE
    ZoneScoped;
    char buffer[1024];
    int len = snprintf(buffer, sizeof(buffer), "Panic reached: %s", message);
    len = std::min(len, (int)(sizeof(buffer) - 1));
    CZ_ASSERT(len >= 0);
    TracyMessage(buffer, len);
#endif

    throw PanicReachedException(message);
}

}
}
