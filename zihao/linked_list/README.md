# Linked list — scouting shortlist

The shortlist of candidates a scout adds players to. Singly linked list, structs only, no classes.

## Files

- `linked_list.h` — the `player` struct, the `Node` struct, the function declarations
- `linked_list.cpp` — the seven functions
- `main.cpp` — a demo that runs every operation once and prints the result
- `CMakeLists.txt` — builds `linklist_demo`

## The seven functions

| Function | What it does |
|---|---|
| `initList` | start empty, `head = nullptr` |
| `addCandidate` | add to the front; a tag that is already there is refused, returns false |
| `removeCandidate` | remove by tag, works for the first node and for a node in the middle |
| `findCandidate` | linear search by tag, returns the node or `nullptr` |
| `printList` | walk from the first node to the last and print each candidate |
| `countCandidates` | count the nodes (a list has no size field) |
| `clearList` | free every node before the program ends |

## Build and run

```
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Release
build/Release/linklist_demo.exe
```

Any compiler that knows C++17 works too:

```
g++ -std=c++17 main.cpp linked_list.cpp -o linklist_demo
```

## Demo output (trimmed)

```
=== 2. add four candidates ===
  1   Echo          Sentinel      84      scouting
  2   Kestrel       Initiator     91      signed
  3   Nyx           Controller    79      scouting
  4   Vortex        Duelist       88      shortlisted

=== 5. remove the FIRST candidate (Echo) ===
  Echo removed, head moved to the next node

=== 6. remove a candidate from the MIDDLE (Kestrel) ===
  Kestrel removed, the node before it now points past it
```

## Two design choices

- **New nodes go to the front.** One step, no other node moves. So the newest candidate prints first.
- **The node stores a copy of the player, not a pointer.** The main squad array reallocates when it grows and gets reordered when it is sorted, so a pointer into it would dangle. A copy cannot.

## When it goes into the group program

`player` is declared again in `linked_list.h` so this folder compiles on its own. Delete that struct during the merge, include `main_functions.h` instead, move the function bodies into `main_functions.cpp` and the declarations into `main_functions.h`. Nothing else needs changing.
