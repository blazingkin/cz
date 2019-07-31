#pragma once

#include "string.hpp"

namespace cz {
namespace format {

enum class Result {
    Ok = 0,
    NotFound,
    PermissionDenied,
    ConnectionRefused,
    ConnectionReset,
    ConnectionAborted,
    NotConnected,
    AddrInUse,
    AddrNotAvailable,
    BrokenPipe,
    AlreadyExists,
    WouldBlock,
    InvalidData,
    TimedOut,
    WriteZero,
    Interrupted,
    UnexpectedEof,
    Other,
};

struct Write {
    Result (*write_str)(void* data, Str str);
};

struct Writer {
    Write write;
    void* data;

    Result write_char(char c);
    Result write_str(Str str);
};

Writer string_writer(String* string);

template <class... Ts>
String format(const char* format, Ts... ts) {
    String string;
    auto writer = string_writer(&string);
    write(writer, format, ts...);
    return string;
}

Result write(Writer writer, char);
Result write(Writer writer, Str);

Result write(Writer writer, short);
Result write(Writer writer, unsigned short);
Result write(Writer writer, int);
Result write(Writer writer, unsigned int);
Result write(Writer writer, long);
Result write(Writer writer, unsigned long);
Result write(Writer writer, long long);
Result write(Writer writer, unsigned long long);

}

inline bool is_err(format::Result r) {
    return r != format::Result::Ok;
}

}
