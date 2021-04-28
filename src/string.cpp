#include <cz/string.hpp>

#include <string.h>
#include <cz/assert.hpp>
#include <cz/util.hpp>

namespace cz {

char* String::buffer() {
    return _buffer;
}
const char* String::buffer() const {
    return _buffer;
}
size_t String::len() const {
    return _len;
}
size_t String::cap() const {
    return _cap;
}

Str String::as_str() const {
    return {_buffer, _len};
}

void String::reserve_total(Allocator allocator, size_t total) {
    if (_cap < total) {
        size_t new_cap = max(total, _cap * 2);
        char* new_buffer;
        if (_buffer) {
            new_buffer = static_cast<char*>(allocator.realloc({_buffer, _cap}, {new_cap, 1}));
        } else {
            new_buffer = static_cast<char*>(allocator.alloc({new_cap, 1}));
        }
        CZ_ASSERT(new_buffer != nullptr);

        _buffer = new_buffer;
        _cap = new_cap;
    }
}

void String::push_many(char ch, size_t count) {
    CZ_DEBUG_ASSERT(_cap - _len >= count);
    memset(_buffer + _len, ch, count);
    _len += count;
    CZ_DEBUG_ASSERT(_len <= _cap);
}

void String::append(Str str) {
    CZ_DEBUG_ASSERT(_cap - _len >= str.len);
    memcpy(_buffer + _len, str.buffer, str.len);
    _len += str.len;
    CZ_DEBUG_ASSERT(_len <= _cap);
}

void String::null_terminate() {
    CZ_DEBUG_ASSERT(_cap - _len >= 1);
    *end() = '\0';
}

void String::insert(size_t index, Str str) {
    CZ_DEBUG_ASSERT(index <= _len);
    CZ_DEBUG_ASSERT(_cap - _len >= str.len);
    memmove(_buffer + index + str.len, _buffer + index, len() - index);
    memcpy(_buffer + index, str.buffer, str.len);
    _len += str.len;
    CZ_DEBUG_ASSERT(_len <= _cap);
}

void String::remove(size_t index, size_t count) {
    CZ_DEBUG_ASSERT(index + count <= _len);
    memmove(_buffer + index, _buffer + index + count, len() - index - count);
    _len -= count;
}

char String::pop() {
    CZ_DEBUG_ASSERT(_len >= 1);
    _len--;
    return _buffer[_len];
}

void String::realloc(Allocator allocator) {
    if (!_buffer) {
        return;
    }

    char* res = static_cast<char*>(allocator.realloc({_buffer, _cap}, {_len, 1}));
    if (res) {
        _buffer = res;
        _cap = _len;
    }
}

void String::realloc_null_terminate(Allocator allocator) {
    char* res;
    if (_buffer) {
        res = static_cast<char*>(allocator.realloc({_buffer, _cap}, {_len + 1, 1}));
    } else {
        CZ_DEBUG_ASSERT(_len == 0);
        res = static_cast<char*>(allocator.alloc({1, 1}));
    }
    CZ_ASSERT(res);
    _buffer = res;
    _buffer[_len] = '\0';
    _cap = _len + 1;
}

void String::set_len(size_t new_len) {
    CZ_DEBUG_ASSERT(new_len <= cap());
    _len = new_len;
}

void String::drop(Allocator allocator) {
    allocator.dealloc({_buffer, _cap});
}

}
