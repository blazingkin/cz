#include <czt/test_base.hpp>

#include <cz/path.hpp>

using namespace cz;
using namespace cz::path;

TEST_CASE("path::directory_component() empty input empty output") {
    REQUIRE(directory_component("") == "");
}

TEST_CASE("path::directory_component() no directory is empty output") {
    REQUIRE(directory_component("abc") == "");
}

TEST_CASE("path::directory_component() gets directory subset") {
    REQUIRE(directory_component("abc/def.txt") == "abc/");
}

TEST_CASE("path::directory_component() gets directory subset multiple directories") {
    REQUIRE(directory_component("abc/def/ghi") == "abc/def/");
}

TEST_CASE("path::directory_component() trailing slash returns input") {
    REQUIRE(directory_component("abc/def/") == "abc/def/");
}

TEST_CASE("path::flatten() empty input empty output") {
    char buffer[3] = {};
    size_t size = 0;

    flatten(buffer, &size);

    CHECK(buffer[0] == 0);
    CHECK(buffer[1] == 0);
    CHECK(buffer[2] == 0);
    CHECK(size == 0);
}

TEST_CASE("path::flatten() no directory is empty output") {
    char buffer[] = "ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("path::flatten() single directory does nothing") {
    char buffer[] = "folder/ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/ooga_booga.txt");
}

TEST_CASE("path::flatten() .. works in middle") {
    char buffer[] = "folder/../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("path::flatten() .. chain works in middle") {
    char buffer[] = "folder1/folder2/../../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("path::flatten() .. chain partially removed in middle") {
    char buffer[] = "folder/../../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "../ooga_booga.txt");
}

TEST_CASE("path::flatten() .. at end resolves to empty string") {
    char buffer[] = "ooga_booga/..";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "");
}

TEST_CASE("path::flatten() .. works at end no trailing slash") {
    char buffer[] = "folder/ooga_booga/..";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/");
}

TEST_CASE("path::flatten() .. works at end with trailing /") {
    char buffer[] = "folder/ooga_booga/../";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/");
}

TEST_CASE("path::flatten() . works middle") {
    char buffer[] = "folder/./ooga_booga";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/ooga_booga");
}

TEST_CASE("path::flatten() . works end") {
    char buffer[] = "folder/ooga_booga/.";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/ooga_booga/");
}

TEST_CASE("path::flatten() . works end with trailing /") {
    char buffer[] = "folder/ooga_booga/./";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/ooga_booga/");
}

TEST_CASE("path::flatten() . then .. works in middle") {
    char buffer[] = "folder/./../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("path::flatten() .. then . works in middle") {
    char buffer[] = "folder/.././ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("path::flatten() ... unchanged") {
    char buffer[] = "folder/.../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "folder/.../ooga_booga.txt");
}

TEST_CASE("path::flatten() starting with ./ stripped") {
    char buffer[] = "./ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "ooga_booga.txt");
}

TEST_CASE("path::flatten() starting with ../ unchanged") {
    char buffer[] = "../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "../ooga_booga.txt");
}

TEST_CASE("path::flatten() starting with multiple ../ unchanged") {
    char buffer[] = "../../ooga_booga.txt";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "../../ooga_booga.txt");
}

#ifdef _WIN32
TEST_CASE("path::flatten() absolute path with drive windows") {
    char buffer[] = "c:/../abc";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "c:/../abc");
}

TEST_CASE("path::flatten() relative path with drive windows") {
    char buffer[] = "c:../abc";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "c:../abc");
}
#endif

TEST_CASE("path::flatten() absolute path") {
    char buffer[] = "/../abc";
    size_t size = sizeof(buffer) - 1;

    flatten(buffer, &size);

    REQUIRE(Str{buffer, size} == "/../abc");
}
