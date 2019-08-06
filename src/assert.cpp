#include "assert.hpp"

#include <stdlib.h>
#include "logger.hpp"

namespace cz {

PanicReachedException::PanicReachedException(SourceLocation loc, const char* message)
    : loc(loc), message(message) {}

void PanicReachedException::log(C* c) {
    log::fatal(c, loc.file, ':', loc.line, ": ", message, '\n');
}

namespace impl {

void panic_reached(SourceLocation loc, const char* message) {
    throw PanicReachedException(loc, message);
}

}
}
