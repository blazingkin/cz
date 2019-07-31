#pragma once

#define CZ_TRY(err)                    \
    do {                               \
        auto error_##__LINE__ = (err); \
        CZ_TRY_VAR(error_##__LINE__);  \
    } while (0)

#define CZ_TRY_VAR(error)        \
    do {                         \
        if (cz::is_err(error)) { \
            return error;        \
        }                        \
    } while (0)

namespace cz {

template <class T>
inline bool is_ok(T&& t) {
    return !is_err(t);
}

inline bool is_err(int e) {
    return e != 0;
}

inline bool is_err(void* v) {
    return v == 0;
}

}
