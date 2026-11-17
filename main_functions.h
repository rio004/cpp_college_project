/*
    main_functions.h
    ------------------------------------------------------------------
    Software Development 1 - Group Assignment (Group 4)
    Application : Esports Scouting & Recruitment System

    This header holds
        - every structure used by the program
        - every function prototype, grouped by report topic
          so the reader can match a function to the assignment topic.

    Rules followed in this project :
        - C++ only, structures only.  No classes anywhere.
        - No input validation.  It is assumed the user types things
          correctly (see "Clarifications" in the report).
*/

#pragma once

#include <iostream>
#include <string>

using namespace std;

// ------------------------------------------------------------------
// Global limits / constants
// ------------------------------------------------------------------
const int MAX_FIXTURES = 20;      // fixtures are kept in a fixed array
const int MAX_TAG_LENGTH = 24;    // used by the shortlist duplicate check
const int DEFAULT_RATING = 50;    // default value used when adding a player

// ------------------------------------------------------------------
// [1] STRUCTURES  (Topic 1 : choice of structures)
// ------------------------------------------------------------------

// A player is somebody the team is watching or has already signed.
struct player
{
    string real_name;       // full name, e.g. "Aaron Byrne"
    string gamer_tag;       // in-game name, e.g. "Vortex"
    string current_role;    // Duelist / Controller / Initiator / Sentinel
    int    rating;          // scouting rating 0 - 100
    float  winRate;         // 0 - 100 (%)
    float  kdRatio;         // kills / deaths
    string scout_status;    // scouting / shortlisted / signed / rejected
};

// One map played inside a fixture.
// topPlayer is a pointer because the MVP is one of the players that
// already exists in the squad array - we do not want to copy him.
struct game
{
    int     gameid;
    string  game;           // map name, e.g. "Ascent"
    player *topPlayer;      // points into the squad array (MVP of the map)
    string  mvp_tag;        // copy of the tag, still valid if the array moves
    int     kills;
    int     deaths;
};

// A fixture = one match against another team, and the map played in it.
struct fixture
{
    int     fixture_id;
    string  opponent;
    string  matchdate;      // stored as "YYYY-MM-DD" so it sorts as text
    game    match_stats;    // nested structure
};

// [9] Linked list node - the shortlist is a singly linked list.
struct ShortlistNode
{
    player        candidate;   // a copy, so the list survives array changes
    ShortlistNode *next;       // nullptr at the end of the list
};

// ------------------------------------------------------------------
// [1] Structure helpers
// ------------------------------------------------------------------
void   initPlayer(player &p, const string &name, const string &tag,
                  const string &role, int rating, float winRate,
                  float kd, const string &status);

// ------------------------------------------------------------------
// [2] FUNCTION DESIGN AND PARAMETER PASSING
// ------------------------------------------------------------------

// [2.1] Passing a variable BY VALUE
//       The function gets a copy, so the original player is never changed.
float calculateBonus(const player p);

// [2.2] Passing an ARRAY of a structure type
//       Arrays decay to a pointer, the size has to be sent separately.
void  printSquad(player squad[], int count);
void  printFixtures(fixture fixtures[], int count);

// [2.3] Passing a variable BY REFERENCE USING POINTERS
void  updateRating(player *p, int delta);
void  updateStatus(player *p, const string &newStatus);

// [2.4] Passing a variable BY REFERENCE USING A REFERENCE VARIABLE (&)
void  applyMatchResult(player &p, int kills, int deaths);

// [2.5] Passing a POINTER BY REFERENCE  (player *&squad)
//       Needed because addPlayer may have to move the array to a bigger
//       block of memory, and that new address must reach the caller.
bool  addPlayer(player *&squad, int &count, int &capacity, const player &p);
void  freeSquad(player *&squad);

// ------------------------------------------------------------------
// [3] MANIPULATING FILE STREAMS (file input / output)
// ------------------------------------------------------------------
int   loadPlayers(player *&squad, int &count, int &capacity,
                  const string &filename);
int   loadFixtures(fixture fixtures[], int maxCount, const string &filename,
                   player squad[], int squadCount);
void  savePlayers(player squad[], int count, const string &filename);

// Refreshes every fixture's MVP pointer after the squad array has been
// reallocated, so no fixture keeps a pointer to freed memory.
void  relinkFixtures(fixture fixtures[], int fixtureCount,
                     player squad[], int squadCount);
void  writeLog(const string &message);

// ------------------------------------------------------------------
// [4] RECURSIVE FUNCTIONS
// ------------------------------------------------------------------
int   totalKills(fixture fixtures[], int count);
int   countShortlist(const ShortlistNode *head);
int   binarySearchRating(player squad[], int low, int high, int target);

// ------------------------------------------------------------------
// [5] FUNCTION OVERLOADING
//       Same name, different parameter lists.
// ------------------------------------------------------------------
void   show(const player &p);                 // full player card
void   show(const game &g);                   // one map of a match
void   show(const fixture &f);                // a whole fixture
player *findPlayer(player squad[], int count, const string &tag);
player *findPlayer(player squad[], int count, int minRating);

// ------------------------------------------------------------------
// [6] DEFAULT VALUES
// ------------------------------------------------------------------
player makePlayer(const string &name, const string &tag, const string &role,
                  int rating = DEFAULT_RATING, float winRate = 0.0f,
                  float kd = 0.0f, const string &status = "scouting");

void   registerPlayer(player *&squad, int &count, int &capacity,
                      const string &name, const string &tag,
                      const string &role, int rating = DEFAULT_RATING);

// ------------------------------------------------------------------
// [7] SORTING ARRAYS OF STRUCTURES
// ------------------------------------------------------------------
void  sortPlayersByRating(player squad[], int count);   // selection sort, desc
void  sortFixturesByDate(fixture fixtures[], int count);

// ------------------------------------------------------------------
// [8] SEARCHING ARRAYS OF STRUCTURES
// ------------------------------------------------------------------
player *searchByTag(player squad[], int count, const string &tag);  // linear
int     searchByRating(player squad[], int count, int target);      // binary

// ------------------------------------------------------------------
// [9] MANIPULATING LINK LISTS OF STRUCTURES
// ------------------------------------------------------------------
void  shortlistAdd(ShortlistNode *&head, const player &p);
bool  shortlistRemove(ShortlistNode *&head, const string &tag);
void  shortlistPrint(const ShortlistNode *head);
void  shortlistClear(ShortlistNode *&head);
bool  shortlistContains(const ShortlistNode *head, const string &tag);

// ------------------------------------------------------------------
// [10] INLINE FUNCTIONS
//       Defined in the header so the compiler can paste the body
//       straight into the calling code (no call overhead).
// ------------------------------------------------------------------
inline bool isStarCandidate(const player &p)
{
    return (p.rating >= 85 && p.kdRatio >= 1.20f && p.winRate >= 60.0f);
}

inline void printDivider()
{
    cout << "----------------------------------------------------\n";
}
