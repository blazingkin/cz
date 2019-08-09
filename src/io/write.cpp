#include "write.hpp"

#include <stdint.h>
#include "../context.hpp"
#include "../try.hpp"

namespace cz {
namespace io {

static Result string_writer_write_str(void* _string, Str str) {
    auto string = static_cast<mem::Allocated<String>*>(_string);
    string->reserve(string->allocator, str.len);
    string->append(str);
    return Result::ok();
}

Writer string_writer(mem::Allocated<String>* string) {
    return {{string_writer_write_str}, string};
}

static Result file_writer_write_str(void* _file, Str str) {
    auto file = static_cast<FILE*>(_file);
    auto ret = fwrite(str.buffer, sizeof(char), str.len, file);

    if (ret == str.len) {
        return Result::ok();
    } else {
        return Result::last_error();
    }
}

Writer file_writer(FILE* file) {
    return {{file_writer_write_str}, file};
}

template <class T>
Result write_base(Writer writer, T val, T base) {
    char buffer[32];
    const char lookup[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t index;
    for (index = sizeof(buffer) - 1; val != 0; --index) {
        buffer[index] = lookup[val % base];
        val /= base;
    }
    ++index;

    return write(writer, {buffer + index, sizeof(buffer) - index});
}

#define define_numeric_write(type)                           \
    Result write(Writer writer, type v) {                    \
        if (v < 0) {                                         \
            CZ_TRY(write(writer, '-'));                      \
            v = -v;                                          \
        }                                                    \
                                                             \
        return write(writer, static_cast<unsigned type>(v)); \
    }                                                        \
                                                             \
    Result write(Writer writer, unsigned type v) {           \
        if (v == 0) {                                        \
            return write(writer, '0');                       \
        }                                                    \
                                                             \
        return write_base<unsigned type>(writer, v, 10);     \
    }

// clang-format off
define_numeric_write(short)
define_numeric_write(int)
define_numeric_write(long)
define_numeric_write(long long)

Result write(Writer writer, Address addr) {
    // clang-format on
    if (addr.val == NULL) {
        return write(writer, "NULL");
    }

    CZ_TRY(write(writer, '0'));
    CZ_TRY(write(writer, 'x'));

    return write_base<intptr_t>(writer, (intptr_t)addr.val, 16);
}

Result write(Writer writer, Debug<Str> str) {
    CZ_TRY(write(writer, '"'));
    for (size_t i = 0; i < str.val.len; ++i) {
        char ch = str.val[i];
        if (ch == '\\') {
            CZ_TRY(write(writer, "\\\\"));
        } else if (ch == '"') {
            CZ_TRY(write(writer, "\\\""));
        } else if (ch == '\n') {
            CZ_TRY(write(writer, "\\n"));
        } else {
            CZ_TRY(write(writer, ch));
        }
    }
    return write(writer, '"');
}

Result write(Writer writer, Debug<MemSlice> slice) {
    return write(writer, "MemSlice{ ", addr(slice.val.buffer), ", ", slice.val.size, " }");
}
Result write(Writer writer, Debug<mem::AllocInfo> info) {
    return write(writer, "AllocInfo{ ", info.val.size, ", ", info.val.alignment, " }");
}

}
}
