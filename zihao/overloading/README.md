# Function overloading and default values

One function name, the compiler picks the version from the arguments. And the last few
parameters of a function can be left out.

## Files

- `overloads.h` — the `player` struct and the declarations
- `overloads.cpp` — two pairs of overloads and `makePlayer` with defaults
- `main.cpp` — a demo that prints which version each call runs
- `CMakeLists.txt` — builds `overloads_demo`

## Overloading: two pairs

| Call | Runs |
|---|---|
| `show(p)` | the six-line detail screen |
| `show(p, 3)` | one numbered row of the list |
| `findPlayer(squad, n, "Nyx")` | search by gamer tag |
| `findPlayer(squad, n, 88)` | search by rating |

The compiler looks at the third argument: a `string` goes to one function, an `int` to the
other. The caller does not have to remember `findPlayerByTag` and `findPlayerByRating`, and
a tag can never be mistaken for a rating.

The return type alone is **not** enough to overload. Same parameter list with a different
return type does not compile, because `show(p);` would be ambiguous.

## Default values

```cpp
player makePlayer(const string &name, const string &tag, const string &role,
                  int rating = 50, float winRate = 0.0f, float kdRatio = 0.0f,
                  const string &status = "scouting");
```

```cpp
makePlayer("Aaron Byrne", "Vortex", "Duelist", 88, 64.5f, 1.32f, "shortlisted"); // everything
makePlayer("Daniel Walsh", "Nyx", "Controller", 79);   // win rate, K/D, status defaulted
makePlayer("Sofia Martins", "Kestrel", "Initiator");   // rating also defaulted, becomes 50
```

A new candidate starts at rating 50 and status "scouting" until the scout fills in real
numbers, which is why the menu lets the rating be skipped with Enter.

Rule: parameters with defaults must sit at the end of the list.

## Build and run

```
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Release
build/Release/overloads_demo.exe
```

## Demo output (trimmed)

```
=== 2. the defaults did their job ===
  Nyx     rating 79, win rate 0, K/D 0, status scouting
  Kestrel rating 50 (no rating was passed in)

=== 5. findPlayer by rating (third argument is an int) ===
  rating 88 belongs to Vortex
```

## When it goes into the group program

Same as the linked list part: delete the `player` struct from `overloads.h`, include
`main_functions.h` instead, and move the functions into `main_functions.cpp`.
