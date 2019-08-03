#include "arena.hpp"

#include <string.h>
#include <memory>
#include "../assert.hpp"
#include "allocator.hpp"
#include "global_allocator.hpp"

namespace cz {
namespace mem {

void Arena::drop() {
    global_allocator.dealloc(mem);
}

static void* advance_ptr_to_alignment(MemSlice old_mem, AllocInfo new_info) {
    // std::align uses references to modify the old variables inplace so is
    // difficult to correctly inline.  This returns true if there is enough room
    // after aligning.
    return std::align(new_info.alignment, new_info.size, old_mem.buffer, old_mem.size);
}

static void* alloc(Arena* arena, AllocInfo info) {
    CZ_DEBUG_ASSERT(arena->mem.buffer != NULL);
    void* result = advance_ptr_to_alignment(
        {(char*)arena->mem.buffer + arena->offset, arena->mem.size - arena->offset}, info);
    if (result) {
        arena->offset = (char*)result - (char*)arena->mem.buffer + info.size;
    }
    return result;
}

static void* arena_realloc(void* _arena, MemSlice old_mem, AllocInfo new_info) {
    auto arena = static_cast<Arena*>(_arena);

    // In place if most recent
    if ((char*)arena->mem.buffer + arena->offset == (char*)old_mem.buffer + old_mem.size) {
        // Realloc in place if enough space
        auto aligned = advance_ptr_to_alignment({old_mem.buffer, arena->mem.size}, new_info);
        if (aligned) {
            arena->offset = (char*)aligned + new_info.size - (char*)arena->mem.buffer;
            return aligned;
        }

        // When in dealloc mode just back out of the end.  We lose the padding
        // added for the alignment of the old_mem but that's ok.
        if (new_info.size == 0) {
            arena->offset = (char*)old_mem.buffer - (char*)arena->mem.buffer;
        }

        // Either deallocating or there isn't enough space.
        return NULL;
    }

    // Allocate as subset of old_mem
    auto old_aligned = advance_ptr_to_alignment(old_mem, new_info);
    if (old_aligned) {
        return old_aligned;
    }

    // Allocate a fresh copy
    auto new_ptr = alloc(arena, new_info);
    if (new_ptr && old_mem.buffer) {
        // Must have a greater new size as smaller sizes are handled above
        CZ_DEBUG_ASSERT(new_info.size <= old_mem.size);
        memcpy(new_ptr, old_mem.buffer, old_mem.size);
    }
    return new_ptr;
}

Allocator Arena::allocator() {
    return {
        arena_realloc,
        this,
    };
}

}
}
