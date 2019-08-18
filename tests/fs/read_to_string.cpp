#include "../test_base.hpp"

#include <cz/defer.hpp>
#include <cz/fs/read_to_string.hpp>
#include <cz/fs/working_directory.hpp>
#include <cz/mem/heap.hpp>
#include "../context.hpp"

using namespace cz;
using namespace cz::fs;
using cz::io::Result;

void set_wd() {
    static bool already_set = false;
    if (already_set) {
        return;
    }

    mem::Allocator allocator = mem::heap_allocator();
    String path;
    CZ_DEFER(path.drop(allocator));

    REQUIRE(!cz::is_err(get_working_directory(allocator, &path)));

    cz::io::put("cwd: ", path);

#if _WIN32
    Str end = "\\out\\build\\x64-Debug";
    if (path.ends_with(end)) {
        path.set_len(path.len() - end.len);
        path.push('\0');
        set_working_directory(path.buffer());
    }
#endif

    already_set = true;
}

TEST_CASE("read_to_string invalid file") {
    set_wd();

    String string;
    auto allocator = mem::heap_allocator();
    CZ_DEFER(string.drop(allocator));

    REQUIRE(read_to_string(allocator, &string, "tests/fs/test_file_that_does_not_exist").is_err());

    REQUIRE(string.len() == 0);
}

TEST_CASE("read_to_string empty file") {
    set_wd();

    String string;
    auto allocator = mem::heap_allocator();
    CZ_DEFER(string.drop(allocator));

    REQUIRE(read_to_string(allocator, &string, "tests/fs/test_empty_file.txt").is_ok());

    REQUIRE(string.len() == 0);
}

TEST_CASE("read_to_string small file") {
    set_wd();

    String string;
    auto allocator = mem::heap_allocator();
    CZ_DEFER(string.drop(allocator));

    REQUIRE(read_to_string(allocator, &string, "tests/fs/test_small_file.txt").is_ok());

    REQUIRE(string == "a\nb\nc\n");
}

TEST_CASE("read_to_string large file") {
    set_wd();

    String string;
    auto allocator = mem::heap_allocator();
    CZ_DEFER(string.drop(allocator));

    REQUIRE(read_to_string(allocator, &string, "tests/fs/test_large_file.txt").is_ok());

    REQUIRE(string ==
            "\
0-0123456789-0\n\
1-0123456789-1\n\
2-0123456789-2\n\
3-0123456789-3\n\
4-0123456789-4\n\
5-0123456789-5\n\
6-0123456789-6\n\
7-0123456789-7\n\
8-0123456789-8\n\
9-0123456789-9\n\
10-0123456789-10\n\
11-0123456789-11\n\
12-0123456789-12\n\
13-0123456789-13\n\
14-0123456789-14\n\
15-0123456789-15\n\
16-0123456789-16\n\
17-0123456789-17\n\
18-0123456789-18\n\
19-0123456789-19\n\
20-0123456789-20\n\
21-0123456789-21\n\
22-0123456789-22\n\
23-0123456789-23\n\
24-0123456789-24\n\
25-0123456789-25\n\
26-0123456789-26\n\
27-0123456789-27\n\
28-0123456789-28\n\
29-0123456789-29\n\
30-0123456789-30\n\
31-0123456789-31\n\
32-0123456789-32\n\
33-0123456789-33\n\
34-0123456789-34\n\
35-0123456789-35\n\
36-0123456789-36\n\
37-0123456789-37\n\
38-0123456789-38\n\
39-0123456789-39\n\
40-0123456789-40\n\
41-0123456789-41\n\
42-0123456789-42\n\
43-0123456789-43\n\
44-0123456789-44\n\
45-0123456789-45\n\
46-0123456789-46\n\
47-0123456789-47\n\
48-0123456789-48\n\
49-0123456789-49\n\
50-0123456789-50\n\
51-0123456789-51\n\
52-0123456789-52\n\
53-0123456789-53\n\
54-0123456789-54\n\
55-0123456789-55\n\
56-0123456789-56\n\
57-0123456789-57\n\
58-0123456789-58\n\
59-0123456789-59\n\
60-0123456789-60\n\
61-0123456789-61\n\
62-0123456789-62\n\
63-0123456789-63\n\
64-0123456789-64\n\
65-0123456789-65\n\
66-0123456789-66\n\
67-0123456789-67\n\
68-0123456789-68\n\
69-0123456789-69\n\
70-0123456789-70\n\
71-0123456789-71\n\
72-0123456789-72\n\
73-0123456789-73\n\
74-0123456789-74\n\
75-0123456789-75\n\
76-0123456789-76\n\
77-0123456789-77\n\
78-0123456789-78\n\
79-0123456789-79\n\
80-0123456789-80\n\
81-0123456789-81\n\
82-0123456789-82\n\
83-0123456789-83\n\
84-0123456789-84\n\
85-0123456789-85\n\
86-0123456789-86\n\
87-0123456789-87\n\
88-0123456789-88\n\
89-0123456789-89\n\
90-0123456789-90\n\
91-0123456789-91\n\
92-0123456789-92\n\
93-0123456789-93\n\
94-0123456789-94\n\
95-0123456789-95\n\
96-0123456789-96\n\
97-0123456789-97\n\
98-0123456789-98\n\
99-0123456789-99\n\
100-0123456789-100\n\
101-0123456789-101\n\
102-0123456789-102\n\
103-0123456789-103\n\
104-0123456789-104\n\
105-0123456789-105\n\
106-0123456789-106\n\
107-0123456789-107\n\
108-0123456789-108\n\
109-0123456789-109\n\
110-0123456789-110\n\
111-0123456789-111\n\
112-0123456789-112\n\
113-0123456789-113\n\
114-0123456789-114\n\
115-0123456789-115\n\
116-0123456789-116\n\
117-0123456789-117\n\
118-0123456789-118\n\
119-0123456789-119\n\
120-0123456789-120\n\
121-0123456789-121\n\
122-0123456789-122\n\
123-0123456789-123\n\
124-0123456789-124\n\
125-0123456789-125\n\
126-0123456789-126\n\
127-0123456789-127\n\
128-0123456789-128\n\
129-0123456789-129\n\
130-0123456789-130\n\
131-0123456789-131\n\
132-0123456789-132\n\
133-0123456789-133\n\
134-0123456789-134\n\
135-0123456789-135\n\
136-0123456789-136\n\
137-0123456789-137\n\
138-0123456789-138\n\
139-0123456789-139\n\
140-0123456789-140\n\
141-0123456789-141\n\
142-0123456789-142\n\
143-0123456789-143\n\
144-0123456789-144\n\
145-0123456789-145\n\
146-0123456789-146\n\
147-0123456789-147\n\
148-0123456789-148\n\
149-0123456789-149\n\
150-0123456789-150\n\
151-0123456789-151\n\
152-0123456789-152\n\
153-0123456789-153\n\
154-0123456789-154\n\
155-0123456789-155\n\
156-0123456789-156\n\
157-0123456789-157\n\
158-0123456789-158\n\
159-0123456789-159\n\
160-0123456789-160\n\
161-0123456789-161\n\
162-0123456789-162\n\
163-0123456789-163\n\
164-0123456789-164\n\
165-0123456789-165\n\
166-0123456789-166\n\
167-0123456789-167\n\
168-0123456789-168\n\
169-0123456789-169\n\
170-0123456789-170\n\
171-0123456789-171\n\
172-0123456789-172\n\
173-0123456789-173\n\
174-0123456789-174\n\
175-0123456789-175\n\
176-0123456789-176\n\
177-0123456789-177\n\
178-0123456789-178\n\
179-0123456789-179\n\
180-0123456789-180\n\
181-0123456789-181\n\
182-0123456789-182\n\
183-0123456789-183\n\
184-0123456789-184\n\
185-0123456789-185\n\
186-0123456789-186\n\
187-0123456789-187\n\
188-0123456789-188\n\
189-0123456789-189\n\
190-0123456789-190\n\
191-0123456789-191\n\
192-0123456789-192\n\
193-0123456789-193\n\
194-0123456789-194\n\
195-0123456789-195\n\
196-0123456789-196\n\
197-0123456789-197\n\
198-0123456789-198\n\
199-0123456789-199\n\
200-0123456789-200\n\
");
}
