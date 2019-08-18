#include <cz/fs/working_directory.hpp>

#include <cz/assert.hpp>
#include <cz/string.hpp>
#include <cz/try.hpp>

#include <errno.h>

// Although the headers are different, we can use the Windows "Posix" implementation to get similar
// semantics.  We have to alias the system calls to the underscore-prefixed versions.  And we have
// to use _MAX_PATH macro instead of using pathconf.
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#else
#include <unistd.h>
#endif

namespace cz {
namespace fs {

io::Result set_working_directory(const char* cstr_path) {
    if (chdir(cstr_path) < 0) {
        return io::Result::last_error();
    } else {
        return io::Result::ok();
    }
}

static io::Result get_path_max(size_t* size) {
#ifdef _WIN32
    *size = _MAX_PATH;
    return io::Result::ok();
#else
    errno = 0;

    long path_max = pathconf("/", _PC_PATH_MAX);

    if (path_max == -1) {
        // errno is only set if there is a limit
        if (errno != 0) {
            return io::Result::last_error();
        } else {
            // @RandomConstant: no limit to the path size
            path_max = 128;
        }
    }

    *size = path_max;

    return io::Result::ok();
#endif
}

io::Result get_working_directory(mem::Allocator allocator, String* path) {
    size_t size;
    CZ_TRY(get_path_max(&size));

    path->clear();
    path->reserve(allocator, size);

    while (!getcwd(path->end(), size)) {
        if (errno == ERANGE) {
            // get longer length and then retry
            size *= 2;
            path->reserve(allocator, size);
        } else {
            // actual error
            return io::Result::last_error();
        }
    }

    path->set_len(strlen(path->buffer()));

#ifdef _WIN32
    // replace '\' with '/' on windows
    for (size_t i = 0; i < path->len(); ++i) {
        if ((*path)[i] == '\\') {
            (*path)[i] = '/';
        }
    }
#endif

    return io::Result::ok();
}

}
}
