#pragma once

#include <stdint.h>
#include <cz/allocator.hpp>
#include <cz/string.hpp>
#include "file.hpp"

#ifdef _WIN32
#else
#include <sys/types.h>
#endif

namespace cz {

struct Process_Options {
    Input_File std_in;
    Output_File std_out;
    Output_File std_err;

    /// The directory to run the process from.
    const char* working_directory = nullptr;

    /// Detach from the parent process instead of becoming a child process.
    bool detach = false;

    void close_all();
};

/// Create a pipe where both ends are inheritable.
bool create_pipe(Input_File*, Output_File*);

/// Create a pipe where the writing side is non-inheritable.  Use this for `Process_Options::stdin`.
bool create_process_input_pipe(Input_File*, Output_File*);
/// Create a pipe where the reading side is non-inheritable.  Use this for `Process_Options::stdout`
/// or `Process_Options::stderr`.
bool create_process_output_pipe(Output_File*, Input_File*);

struct Process_IOE {
    Output_File std_in;
    Input_File std_out;
    Input_File std_err;
};

struct Process_IO {
    Output_File std_in;
    Input_File std_out;
};

bool create_process_pipes(Process_IO*, Process_Options*);
bool create_process_pipes(Process_IOE*, Process_Options*);

struct Process {
private:
#ifdef _WIN32
    void* hProcess;
#else
    pid_t pid;
#endif

public:
    /// Escape one `arg` (argument) and append it to the `string`.
    ///
    /// Leaves an unallocated spot at the end of the `string` to either append a space (to split
    /// arguments) or a null terminator (to complete the script).
    static void escape_arg(cz::Str arg, cz::String* string, cz::Allocator allocator);

    /// Form a `script` from the list of `args` (arguments).
    ///
    /// Null terminates the script.
    static void escape_args(cz::Slice<const cz::Str> args,
                            cz::String* script,
                            cz::Allocator allocator);

    /// Launch a program.  The first argument must be the same the path to invoke.
    ///
    /// The process's `stdin`, `stdout`, and `stderr` streams are bound to the `options`' streams.
    /// The streams in `options` are not closed by this function.  Any files that are null (the
    /// default) are closed instead of being bound (in the new process).
    ///
    /// The return value is `true` if the program was successfully launched.
    bool launch_program(cz::Slice<const cz::Str> args, Process_Options* options);

    /// Launch a script as if it was ran on the command line.
    ///
    /// This runs the script through `cmd` on Windows and `/bin/sh` otherwise.
    ///
    /// See also `launch_program` for information on how `options` are handled.
    bool launch_script(cz::Str script, Process_Options* options);

    /// Kill the child process and destroy the `Process`.
    void kill();

    /// Wait for the child process to end if it hasn't ended already and destroy the `Process`.
    ///
    /// Returns the exit code of the process or a special code if the process terminated
    /// without exiting (crashed).  On Windows this is `-1`, on Linux this is `127`.
    int join();

    /// Try to join the process, returning whether it was successful.
    ///
    /// If it hasn't completed, returns `false`.  If it has completed then stores the
    /// exit code in `exit_code` (ie the return value of `join`) and returns `true`.
    ///
    /// See `join` for more information.
    bool try_join(int* exit_code);

    /// Destroy the `Process` but don't terminate the child process.
    void detach();
};

}
