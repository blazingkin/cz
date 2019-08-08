#include "heap.hpp"

#include <stdlib.h>

namespace cz {
namespace mem {

static MemSlice heap_alloc(void*, AllocInfo info) {
    // TODO alignment
    auto buf = malloc(info.size);
    if (buf) {
        return {buf, info.size};
    } else {
        return {NULL, 0};
    }
}

static void heap_dealloc(void*, MemSlice mem) {
    free(mem.buffer);
}

static MemSlice heap_realloc(void*, MemSlice old_mem, AllocInfo new_info) {
    // TODO alignment
    auto buf = realloc(old_mem.buffer, new_info.size);
    if (buf) {
        return {buf, new_info.size};
    } else {
        return {NULL, 0};
    }
}

Allocator heap_allocator() {
    return {{heap_alloc, heap_dealloc, heap_realloc}, NULL};
}

}
}
