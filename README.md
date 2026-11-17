# Esports Scouting and Recruitment System (Group 4)

Software Development 1 group assignment. This folder is a complete, runnable version of the
program plus the individual report, ready to be copied into
`https://github.com/rio004/cpp_college_project`.

No classes anywhere - only structures, arrays of structures and a linked list, as the brief asks.

---

## 1. What is in this folder

| Path | What it is |
| --- | --- |
| `main_functions.h` | Structures (`player`, `game`, `fixture`, `ShortlistNode`), constants and all prototypes, plus the two inline functions |
| `main_functions.cpp` | Every function, grouped and commented in the order of the report topics |
| `main.cpp` | The menu, the screens and the input helpers (dynamic squad array) |
| `function_tests.cpp` | 14 Google Test unit tests (BVA on `isStarCandidate` and one test per topic) |
| `CMakeLists.txt` | Builds `main_app` and `scout_tests`, downloads Google Test with FetchContent |
| `data/players.txt` | Six starting players, one per line: `tag|name|role|rating|winRate|kd|status` |
| `data/fixtures.txt` | Five starting fixtures: `id|opponent|date|map|kills|deaths|mvpTag` |
| `Software_development_1_Assignment_E-sport_Gaming.docx` | The individual report (topics 0A, 0B, 1-12 and both appendices) |
| `docs/demo_inputs.txt` | The keystrokes used for the demo run |
| `docs/demo_transcript.txt` | The full console output of that run (the source of the Appendix 2 figures) |
| `docs/test_output.txt` | Output of the test executable, all 14 tests passing |
| `docs/figures/` | The 11 console screenshots used in Appendix 2 |
| `docs/make_figures.py`, `docs/build_report.py` | Scripts that regenerate the figures and the report from the live transcript and source files |

`data/players_out.txt` and `data/scout_log.txt` are produced by running the program; they can be
deleted before committing.

---

## 2. How to build and run

Any C++17 compiler works. With CMake (the same way the original repository is set up):

```
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Release
```

Then:

```
build\Release\main_app.exe          # the program
build\Release\scout_tests.exe       # the unit tests
```

Building inside a path that contains Chinese characters can upset the test-discovery step of
CMake, so if `ctest` complains, build in a plain path such as `D:/scout_build`:

```
cmake -S . -B D:/scout_build -G "Visual Studio 18 2026" -A x64
cmake --build D:/scout_build --config Release
```

Visual Studio also opens `CMakeLists.txt` directly ("Open Folder"), which needs no command line.

---

## 3. How to demonstrate it in the lab

| Key | What it shows | Topic it demonstrates |
| --- | --- | --- |
| 1 | Squad table, `*` marks star candidates | array of structures, inline function, overloading (`show`) |
| 2 | Register a player, press Enter to accept the default rating | default arguments, pointer by reference, parameter passing |
| 3 | Log a match result (kills, deaths) | reference parameter, function design |
| 4 | Sort by rating, highest first | sorting an array of structures, relinking the fixtures |
| 5 | Find by gamer tag | linear search, pointer return, overload resolution |
| 6 | Find by rating | recursive binary search (sorts first) |
| 7 | Shortlist: add, show (with the recursive count), remove | linked list of structures, recursion on the list |
| 8 | Fixtures: list, sort by date, total kills (recursion), detail | recursion, file input, overloading |
| 9 | Save | file output |
| 10 | Reload | file input, dynamic array growth |
| 0 | Exit | clean-up (`delete[]`, list freed) |

`docs/demo_inputs.txt` replays exactly the run shown in Appendix 2:

```
main_app.exe < docs\demo_inputs.txt
```

---

## 4. How to submit

The program has to be pushed and the report uploaded by you, from your own accounts - I cannot
do that part.

1. **Get your clone up to date**

   ```
   git clone https://github.com/rio004/cpp_college_project
   cd cpp_college_project
   ```

   If you already have it, `git pull` first so you do not fight someone else's commit.

2. **Copy the files in** (overwrite the four placeholder files, keep the new ones):

   ```
   copy <this folder>\main_functions.h      .
   copy <this folder>\main_functions.cpp    .
   copy <this folder>\main.cpp              .
   copy <this folder>\function_tests.cpp    .
   copy <this folder>\CMakeLists.txt        .
   xcopy <this folder>\data  data\ /E /I
   ```

   Commit with a message that says what it is, for example
   `Add working menu, linked list, overloading and unit tests`, then `git push`.
   Check the build once more from a fresh clone so you know nothing was left behind.

3. **Upload the report** to the SharePoint/Teams drop-box the module uses, as a Word document:
   `Software_development_1_Assignment_E-sport_Gaming.docx`. Keep the file name sensible - the
   module wants your name and student number to be visible, and they are on the cover page.

4. **Before you upload, read the report once and change what is not true for you.** The `(b) My
   contribution` paragraphs follow the task table in the shared spreadsheet (you: screen/menu,
   linked list, overloading, default values, binary search, testing). If a teammate actually wrote
   something different, say so in your own words - that section is marked on your explanation of
   what you personally did.

5. **Demonstrate** the program in the lab on Tuesday 1 December 2026. Every member has to be
   there and can be asked individual questions, so be ready to explain any screen, not only your
   own functions.

---

## 5. Known limitations (say so in the report rather than hide it)

- No input validation, as the brief allows - typing a letter where a number is expected simply
  ends the read and leaves the value at zero.
- The squad array grows by doubling, so the order of `fixture.match_stats.topPlayer` has to be
  restored with `relinkFixtures` after a registration or a sort; that is the bug discussed in
  topic [12].
- Ratings are whole numbers, so the binary search can only find a rating that exists; the menu
  sorts first and tells the user that it did.
