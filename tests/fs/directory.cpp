#include <czt/test_base.hpp>

#include <cz/defer.hpp>
#include <cz/fs/directory.hpp>
#include <cz/mem/heap.hpp>
#include <cz/string.hpp>

using namespace cz;
using namespace cz::mem;
using namespace cz::fs;

void set_wd();

TEST_CASE("fs::files works") {
    set_wd();

    Allocator allocator = heap_allocator();

    const char* dir = "tests/fs";

    DirectoryIterator iterator(allocator);
    REQUIRE(iterator.create(dir).is_ok());
    CZ_DEFER(REQUIRE(iterator.destroy().is_ok()));

    SmallVector<String, 0> paths;
    CZ_DEFER(paths.drop(allocator));
    CZ_DEFER(for (size_t i = 0; i < paths.len(); ++i) { paths[i].drop(allocator); });
    REQUIRE(files(allocator, dir, &paths).is_ok());

    size_t i = 0;
    while (!iterator.done()) {
        cz::println("ls: ", iterator.file());
        REQUIRE(iterator.file() == paths[i++]);
        REQUIRE(iterator.advance().is_ok());
    }
}

TEST_CASE("fs::directory_component() empty input empty output") {
    REQUIRE(directory_component("") == "");
}

TEST_CASE("fs::directory_component() no directory is empty output") {
    REQUIRE(directory_component("abc") == "");
}

TEST_CASE("fs::directory_component() gets directory subset") {
    REQUIRE(directory_component("abc/def.txt") == "abc/");
}

TEST_CASE("fs::directory_component() gets directory subset multiple directories") {
    REQUIRE(directory_component("abc/def/ghi") == "abc/def/");
}

TEST_CASE("fs::directory_component() trailing slash returns input") {
    REQUIRE(directory_component("abc/def/") == "abc/def/");
}

TEST_CASE("fs::flatten_path() empty input empty output") {
    char buffer[3] = {};
    size_t size = 0;

    flatten_path(buffer, &size);

    CHECK(buffer[0] == 0);
    CHECK(buffer[1] == 0);
    CHECK(buffer[2] == 0);
    CHECK(size == 0);
}

TEST_CASE("fs::flatten_path() no directory is empty output") {
    char buffer[] = "ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() single directory does nothing") {
    char buffer[] = "folder/ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() .. works in middle") {
    char buffer[] = "folder/../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() .. chain works in middle") {
    char buffer[] = "folder1/folder2/../../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() .. chain partially removed in middle") {
    char buffer[] = "folder/../../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "../ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() .. at end resolves to empty string") {
    char buffer[] = "ooga_booga/..";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "");
}

TEST_CASE("fs::flatten_path() .. works at end no trailing slash") {
    char buffer[] = "folder/ooga_booga/..";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/");
}

TEST_CASE("fs::flatten_path() .. works at end with trailing /") {
    char buffer[] = "folder/ooga_booga/../";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/");
}

TEST_CASE("fs::flatten_path() . works middle") {
    char buffer[] = "folder/./ooga_booga";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/ooga_booga");
}

TEST_CASE("fs::flatten_path() . works end") {
    char buffer[] = "folder/ooga_booga/.";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/ooga_booga/");
}

TEST_CASE("fs::flatten_path() . works end with trailing /") {
    char buffer[] = "folder/ooga_booga/./";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/ooga_booga/");
}

TEST_CASE("fs::flatten_path() . then .. works in middle") {
    char buffer[] = "folder/./../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() .. then . works in middle") {
    char buffer[] = "folder/.././ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() ... unchanged") {
    char buffer[] = "folder/.../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/.../ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() starting with ./ stripped") {
    char buffer[] = "./ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() starting with ../ unchanged") {
    char buffer[] = "../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "../ooga_booga.txt");
}

TEST_CASE("fs::flatten_path() starting with multiple ../ unchanged") {
    char buffer[] = "../../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten_path(buffer, &size);

    REQUIRE(Str{buffer, size} == "../../ooga_booga.txt");
}
