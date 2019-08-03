#include "write.hpp"

#include <stdint.h>
#include "../try.hpp"

namespace cz {
namespace io {

static Result string_writer_write_str(void* _string, Str str) {
    auto string = static_cast<String*>(_string);
    string->append(str);
    return Result::ok();
}

Writer string_writer(String* string) {
    return {{string_writer_write_str}, string};
}

static Result file_writer_write_str(void* _file, Str str) {
    auto file = static_cast<FILE*>(_file);
    auto ret = fwrite(str.buffer, sizeof(char), str.len, file);

    if (ret == str.len) {
        return Result::ok();
    } else {
        return Result::from_errno();
    }
}

Writer file_writer(FILE* file) {
    return {{file_writer_write_str}, file};
}

#define define_numeric_write(type)                                      \
    Result write(Writer writer, type v) {                               \
        if (v < 0) {                                                    \
            CZ_TRY(write(writer, '-'));                                 \
            v = -v;                                                     \
        }                                                               \
                                                                        \
        return write(writer, static_cast<unsigned type>(v));            \
    }                                                                   \
                                                                        \
    Result write(Writer writer, unsigned type v) {                      \
        if (v == 0) {                                                   \
            return write(writer, '0');                                  \
        }                                                               \
                                                                        \
        char buffer[32];                                                \
        size_t index;                                                   \
        for (index = sizeof(buffer) - 1; v != 0; --index) {             \
            buffer[index] = '0' + v % 10;                               \
            v /= 10;                                                    \
        }                                                               \
        ++index;                                                        \
                                                                        \
        return write(writer, {buffer + index, sizeof(buffer) - index}); \
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

    auto v = (intptr_t)addr.val;
    char buffer[32];
    size_t index;
    for (index = 0; v != 0; ++index) {
        buffer[index] = '0' + v % 16;
        v /= 16;
    }
    while (index > 0) {
        --index;
        CZ_TRY(write(writer, buffer[index]));
    }
    return Result::ok();
}

}
}
