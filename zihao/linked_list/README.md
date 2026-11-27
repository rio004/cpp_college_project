# Linked list — scouting shortlist

The shortlist a scout keeps while looking for new players. Singly linked list of structs,
no classes.

## Files

- `linked_list.h` — the `player` struct, the `Node` struct, the declarations
- `linked_list.cpp` — the eight functions
- `main.cpp` — a demo that runs every operation once
- `CMakeLists.txt` — builds `linklist_demo`

## The eight functions

| Function | What it does |
|---|---|
| `initList` | start empty, `head = nullptr` |
| `addCandidate` | add to the front; a tag that is already there is refused, returns false |
| `popCandidate` | take the first candidate off the list, hand the record back, free the node |
| `removeCandidate` | remove by tag, works for the first node and for a node in the middle |
| `findCandidate` | linear search by tag, returns the node or `nullptr` |
| `showCandidate` | print the full record of one player |
| `printList` | walk from the first node to the last |
| `countCandidates` | count the nodes (a list has no size field) |
| `clearList` | free every node before the program ends |

`popCandidate` is the one we talked about: the node leaves the list but the player record
comes back in the `out` parameter, so the menu can print who was popped and then decide
whether to sign him.

```cpp
player popped;
if (popCandidate(shortlist, popped))
{
    showCandidate(popped);
}
```

## Field names and status values

`player` here is the same struct as in `main_functions.h` on `main`: `playerid` first, and
`scout_status` is `"Signed"` or `"Unsigned"` (`Signed` = already has a team, `Unsigned` =
still available). Test data copied from `existingPlayers.csv`.

## Build and run

```
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Release
build/Release/linklist_demo.exe
```

Any C++17 compiler works too: `g++ -std=c++17 main.cpp linked_list.cpp -o linklist_demo`.

## Demo output (trimmed)

```
[2] add four candidates (each one goes to the front)
  No  ID   Gamer tag     Role          Rating  Status
  1   89   Oner          Jungle        93      Unsigned
  2   45   Zeus          Top           92      Unsigned
  3   6    Keria         Support       94      Signed
  4   7    Faker         Mid           96      Signed

[5] pop the first candidate -> record printed, node deleted
    popped: Oner
  Player ID : 89
  Name      : Moon Hyeon-jun
  ...
  Status    : Unsigned
```

## Two design choices

- **New nodes go to the front.** One step, no other node moves, so the newest candidate
  prints first.
- **The node stores a copy of the player, not a pointer.** The squad array reallocates when
  it grows and gets reordered when it is sorted, so a pointer into it would dangle.

## When it goes into the group program

`player` is declared again in `linked_list.h` so this folder compiles alone. At merge time
delete that struct, include `main_functions.h`, move the bodies into `main_functions.cpp`
and the declarations into `main_functions.h`.
