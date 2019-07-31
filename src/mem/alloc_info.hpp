#pragma once

#include <stddef.h>

namespace cz {
namespace mem {

struct AllocInfo {
    size_t size;
    size_t alignment;

    constexpr bool operator==(const AllocInfo& other) const {
        return size == other.size && alignment == other.alignment;
    }
    constexpr bool operator!=(const AllocInfo& other) const { return !(*this == other); }
};

template <class T>
AllocInfo alloc_info() {
    return {sizeof(T), alignof(T)};
}

}
}
