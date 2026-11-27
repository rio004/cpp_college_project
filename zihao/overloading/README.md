# Function overloading and default values

One name, the compiler picks the version from the arguments. And the last parameters of a
function can be left out.

## Files

- `overloads.h` — the `player` struct and the declarations
- `overloads.cpp` — two pairs of overloads plus `makePlayer` with defaults
- `main.cpp` — a demo that prints which version each call runs
- `CMakeLists.txt` — builds `overloads_demo`

## Overloading: two pairs

| Call | Runs |
|---|---|
| `show(p)` | the eight-line detail screen |
| `show(p, 3)` | one numbered row of the list |
| `findPlayer(squad, n, "Zeus")` | search by gamer tag |
| `findPlayer(squad, n, 89)` | search by player id |

The compiler looks at the third argument: a `string` goes to one function, an `int` to the
other. The caller doesn't have to remember two names, and a tag can never be mistaken for
an id.

The return type alone is **not** enough to overload. Same parameter list with a different
return type does not compile, because `show(p);` would be ambiguous.

## Default values

```cpp
player makePlayer(int playerid, const string &name, const string &tag, const string &role,
                  int rating = 50, float winRate = 0.0f, float kdRatio = 0.0f,
                  const string &status = "Unsigned");
```

```cpp
makePlayer(7,  "Lee Sang-hyeok", "Faker", "Mid", 96, 67.4f, 4.10f, "Signed"); // everything
makePlayer(45, "Choi Woo-je",    "Zeus",  "Top");  // rating, win rate, K/D, status defaulted
```

A new candidate starts at rating 50 and status `Unsigned` until the scout fills in real
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
[4] show(player, index) - one line of the list
  [0] 7   Faker       Mid         96  Signed
  [1] 45  Zeus        Top         92  Unsigned
  [2] 89  Oner        Jungle      93  Unsigned
  [3] 120 Rookie      Support     50  Unsigned

[7] a tag and an id that are not there
  findPlayer(tag 'Nyx')   -> -1
  findPlayer(id 999)      -> -1
```

The last line of the list is the one made with defaults: rating 50, status `Unsigned`.

## When it goes into the group program

Same as the linked list part: delete the `player` struct from `overloads.h`, include
`main_functions.h`, move the functions into `main_functions.cpp`. `show(player)` and the
list row are for the menu; `findPlayer` can stay next to `searchForPlayerID`.
