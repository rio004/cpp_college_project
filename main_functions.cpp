/*
    main_functions.cpp
    ------------------------------------------------------------------
    Implementation of every function declared in main_functions.h.

    The functions are written in report order (Topic 1 -> Topic 12)
    so the code in the appendix can be read next to the report.
*/

#include "main_functions.h"

#include <fstream>
#include <iomanip>
#include <sstream>

// ------------------------------------------------------------------
// [1] STRUCTURES
// ------------------------------------------------------------------

// Fill one player with the given values.
// A reference is used (not a pointer) because a player object must
// already exist before this is called - nothing is created here.
void initPlayer(player &p, const string &name, const string &tag,
                const string &role, int rating, float winRate,
                float kd, const string &status)
{
    p.real_name    = name;
    p.gamer_tag    = tag;
    p.current_role = role;
    p.rating       = rating;
    p.winRate      = winRate;
    p.kdRatio      = kd;
    p.scout_status = status;
}

// ------------------------------------------------------------------
// [2] FUNCTION DESIGN AND PARAMETER PASSING
// ------------------------------------------------------------------

// [2.1] Passing a variable by value.
// The player is copied into p, so no matter what happens inside this
// function the caller's player is untouched.  That is exactly what we
// want for a "what if" calculation: a scout can ask "what would this
// player be worth?" without changing the stored record.
float calculateBonus(const player p)
{
    float bonus = p.rating * 0.8f;          // rating carries most of the weight
    bonus = bonus + p.kdRatio * 6.0f;       // fragging power
    bonus = bonus + p.winRate * 0.4f;       // winning matters
    if (p.scout_status == "signed")
        bonus = bonus - 20.0f;              // already signed - less urgent
    return bonus;
}

// [2.2] Passing an array of a structure type.
// In C++ an array name decays to a pointer, therefore the function
// receives the address of the first element and the count must be
// passed separately.  The array is printed in a table.
void printSquad(player squad[], int count)
{
    cout << left
         << setw(4)  << "No"
         << setw(22) << "Gamer tag"
         << setw(14) << "Role"
         << setw(7)  << "Rating"
         << setw(8)  << "Win %"
         << setw(7)  << "K/D"
         << setw(12) << "Status"
         << "Star" << "\n";
    printDivider();

    for (int i = 0; i < count; i++)
    {
        cout << left
             << setw(4)  << (i + 1)
             << setw(22) << squad[i].gamer_tag
             << setw(14) << squad[i].current_role
             << setw(7)  << squad[i].rating
             << setw(8)  << fixed << setprecision(1) << squad[i].winRate
             << setw(7)  << setprecision(2) << squad[i].kdRatio
             << setw(12) << squad[i].scout_status
             << (isStarCandidate(squad[i]) ? "*" : "-") << "\n";
    }
    cout.unsetf(ios::fixed);          // back to normal number formatting
    cout.precision(6);
    cout << "\n(" << count << " player(s) on the scouting list)\n";
}

void printFixtures(fixture fixtures[], int count)
{
    cout << left
         << setw(4)  << "ID"
         << setw(18) << "Opponent"
         << setw(13) << "Date"
         << setw(14) << "Map"
         << setw(8)  << "Kills"
         << setw(8)  << "Deaths"
         << "MVP" << "\n";
    printDivider();

    for (int i = 0; i < count; i++)
    {
        const game &g = fixtures[i].match_stats;   // reference, no copy
        cout << left
             << setw(4)  << fixtures[i].fixture_id
             << setw(18) << fixtures[i].opponent
             << setw(13) << fixtures[i].matchdate
             << setw(14) << g.game
             << setw(8)  << g.kills
             << setw(8)  << g.deaths
             << (g.topPlayer ? g.topPlayer->gamer_tag : string("-")) << "\n";
    }
}

// [2.3] Passing a variable by reference using pointers.
// A pointer is used because the function is allowed to do nothing
// (p can be nullptr if the player was not found), which a reference
// cannot express.
void updateRating(player *p, int delta)
{
    if (p == nullptr)
        return;                       // nothing to update

    p->rating = p->rating + delta;
    if (p->rating > 100) p->rating = 100;
    if (p->rating < 0)   p->rating = 0;
}

void updateStatus(player *p, const string &newStatus)
{
    if (p == nullptr)
        return;
    p->scout_status = newStatus;
}

// [2.4] Passing a variable by reference using a reference variable.
// applyMatchResult always needs a real player (there is no "no result"
// case), so a reference is the honest choice: it cannot be null and
// the caller's object is updated directly.
void applyMatchResult(player &p, int kills, int deaths)
{
    int safeDeaths = (deaths == 0) ? 1 : deaths;      // avoid divide by zero
    float mapKd = (float)kills / (float)safeDeaths;

    p.kdRatio = (p.kdRatio + mapKd) / 2.0f;           // rolling average

    if (mapKd >= 1.0f)                                // winning form
    {
        p.rating = p.rating + 2;
        if (p.rating > 100) p.rating = 100;
        p.winRate = p.winRate + 1.0f;
        if (p.winRate > 100.0f) p.winRate = 100.0f;
    }
    else
    {
        updateRating(&p, -1);                         // pointer version reused
    }
}

// [2.5] Passing a pointer by reference.  player *&squad
// The squad is a dynamic array.  When it is full the array must be
// deleted and a bigger one allocated.  The new address only reaches
// the caller if the pointer itself is passed by reference - passing
// player *squad would copy the pointer and the caller would keep
// pointing at freed memory.
bool addPlayer(player *&squad, int &count, int &capacity, const player &p)
{
    if (count >= capacity)                            // no room left
    {
        int newCapacity = capacity * 2;
        player *bigger = new player[newCapacity];

        for (int i = 0; i < count; i++)               // copy the old records
            bigger[i] = squad[i];

        delete[] squad;                               // old block is gone
        squad    = bigger;                            // <-- why *& is needed
        capacity = newCapacity;
    }

    squad[count] = p;
    count++;
    return true;
}

void freeSquad(player *&squad)
{
    delete[] squad;
    squad = nullptr;
}

// ------------------------------------------------------------------
// [3] MANIPULATING FILE STREAMS
// ------------------------------------------------------------------

// Read "tag|name|role|rating|winRate|kd|status" lines.
// Returns how many players were added.
int loadPlayers(player *&squad, int &count, int &capacity,
                const string &filename)
{
    ifstream inFile(filename.c_str());
    if (!inFile.is_open())
    {
        cout << "[file] could not open " << filename << "\n";
        return 0;
    }

    string line;
    int added = 0;

    while (getline(inFile, line))
    {
        if (line.empty() || line[0] == '#')
            continue;                                  // comments / blank lines

        stringstream ss(line);
        string tag, name, role, ratingText, winText, kdText, status;

        getline(ss, tag,    '|');
        getline(ss, name,   '|');
        getline(ss, role,   '|');
        getline(ss, ratingText, '|');
        getline(ss, winText,    '|');
        getline(ss, kdText,     '|');
        getline(ss, status,     '|');

        if (tag.empty() || ratingText.empty())
            continue;                                  // malformed record

        player p;
        initPlayer(p, name, tag, role, atoi(ratingText.c_str()),
                   (float)atof(winText.c_str()),
                   (float)atof(kdText.c_str()),
                   status.empty() ? "scouting" : status);

        addPlayer(squad, count, capacity, p);
        added++;
    }

    inFile.close();
    return added;
}

// "id|opponent|date|map|kills|deaths|mvpTag"
// The MVP is stored as a tag and converted to a pointer on load, so
// the fixture points straight at the player object in the squad array.
int loadFixtures(fixture fixtures[], int maxCount, const string &filename,
                 player squad[], int squadCount)
{
    ifstream inFile(filename.c_str());
    if (!inFile.is_open())
    {
        cout << "[file] could not open " << filename << "\n";
        return 0;
    }

    string line;
    int count = 0;

    while (getline(inFile, line) && count < maxCount)
    {
        if (line.empty() || line[0] == '#')
            continue;

        stringstream ss(line);
        string idText, opponent, date, mapName, killsText, deathsText, mvpTag;

        getline(ss, idText,    '|');
        getline(ss, opponent,  '|');
        getline(ss, date,      '|');
        getline(ss, mapName,   '|');
        getline(ss, killsText, '|');
        getline(ss, deathsText,'|');
        getline(ss, mvpTag,    '|');

        fixture f;
        f.fixture_id         = atoi(idText.c_str());
        f.opponent           = opponent;
        f.matchdate          = date;
        f.match_stats.gameid = f.fixture_id;
        f.match_stats.game   = mapName;
        f.match_stats.kills  = atoi(killsText.c_str());
        f.match_stats.deaths = atoi(deathsText.c_str());
        f.match_stats.mvp_tag = mvpTag;  // keep the name, the array may move

        // Search the squad so the nested pointer is real, not a dangling one.
        player *mvp = findPlayer(squad, squadCount, mvpTag);
        f.match_stats.topPlayer = mvp;   // nullptr if the tag is unknown

        fixtures[count] = f;
        count++;
    }

    inFile.close();
    return count;
}

/*
    relinkFixtures
    ------------------------------------------------------------------
    Every fixture points at one player (its MVP) inside the squad
    array.  When a new player makes that array grow, addPlayer() throws
    the old block away and the pointers inside the fixtures would point
    at freed memory.  This function looks the MVP up again from the tag
    stored in the fixture, so the pointers are valid once more.
*/
void relinkFixtures(fixture fixtures[], int fixtureCount,
                    player squad[], int squadCount)
{
    for (int i = 0; i < fixtureCount; i++)
    {
        fixtures[i].match_stats.topPlayer =
            findPlayer(squad, squadCount, fixtures[i].match_stats.mvp_tag);
    }
}

void savePlayers(player squad[], int count, const string &filename)
{
    ofstream outFile(filename.c_str());          // truncates the old file
    if (!outFile.is_open())
    {
        cout << "[file] could not write " << filename << "\n";
        return;
    }

    outFile << "# tag|name|role|rating|winRate|kd|status\n";
    for (int i = 0; i < count; i++)
    {
        outFile << squad[i].gamer_tag    << "|"
                << squad[i].real_name    << "|"
                << squad[i].current_role << "|"
                << squad[i].rating       << "|"
                << squad[i].winRate      << "|"
                << squad[i].kdRatio      << "|"
                << squad[i].scout_status << "\n";
    }

    outFile.close();
    cout << "[file] " << count << " player(s) written to " << filename << "\n";
}

// Append mode: the log grows, it never overwrites.
void writeLog(const string &message)
{
    ofstream logFile("data/scout_log.txt", ios::app);
    if (!logFile.is_open())
        return;
    logFile << message << "\n";
    logFile.close();
}

// ------------------------------------------------------------------
// [4] RECURSIVE FUNCTIONS
// ------------------------------------------------------------------

// Sum of kills of every fixture.
// Base case: no fixtures left -> 0.
int totalKills(fixture fixtures[], int count)
{
    if (count <= 0)
        return 0;                                  // base case
    return fixtures[count - 1].match_stats.kills   // one element
         + totalKills(fixtures, count - 1);        // the rest
}

// Length of the linked list, counted recursively.
int countShortlist(const ShortlistNode *head)
{
    if (head == nullptr)
        return 0;                                  // base case
    return 1 + countShortlist(head->next);         // this node + the rest
}

// Recursive binary search.
// The squad is sorted HIGHEST RATING FIRST, so every value to the left
// is larger and every value to the right is smaller - the comparison
// is therefore reversed compared to the textbook version.
int binarySearchRating(player squad[], int low, int high, int target)
{
    if (low > high)
        return -1;                                 // not present

    int mid = low + (high - low) / 2;

    if (squad[mid].rating == target)
        return mid;                                // found

    if (squad[mid].rating < target)
        return binarySearchRating(squad, low, mid - 1, target);

    return binarySearchRating(squad, mid + 1, high, target);
}

// ------------------------------------------------------------------
// [5] FUNCTION OVERLOADING
//       The same name "show" with three different parameter types.
//       The compiler picks the right one from the argument type.
// ------------------------------------------------------------------

void show(const player &p)
{
    cout << "  Name      : " << p.real_name << "\n"
         << "  Tag       : " << p.gamer_tag << "\n"
         << "  Role      : " << p.current_role << "\n"
         << "  Rating    : " << p.rating << "\n"
         << "  Win rate  : " << fixed << setprecision(1) << p.winRate << " %\n"
         << "  K/D ratio : " << setprecision(2) << p.kdRatio << "\n"
         << "  Status    : " << p.scout_status << "\n"
         << "  Star?     : " << (isStarCandidate(p) ? "yes" : "no") << "\n";
    cout.unsetf(ios::fixed);
    cout.precision(6);
}

void show(const game &g)
{
    cout << "  Map       : " << g.game << " (game id " << g.gameid << ")\n"
         << "  Kills     : " << g.kills << "\n"
         << "  Deaths    : " << g.deaths << "\n"
         << "  MVP       : "
         << (g.topPlayer ? g.topPlayer->gamer_tag : string("not recorded"))
         << "\n";
}

void show(const fixture &f)
{
    cout << "  Fixture   : #" << f.fixture_id << " vs " << f.opponent << "\n"
         << "  Date      : " << f.matchdate << "\n";
    show(f.match_stats);        // overload resolution again, on the game
}

// Overload 1 : by tag (linear search, the array is not sorted by tag)
player *findPlayer(player squad[], int count, const string &tag)
{
    for (int i = 0; i < count; i++)
    {
        if (squad[i].gamer_tag == tag)
            return &squad[i];                 // address of the real record
    }
    return nullptr;
}

// Overload 2 : by minimum rating (returns the first acceptable player)
player *findPlayer(player squad[], int count, int minRating)
{
    for (int i = 0; i < count; i++)
    {
        if (squad[i].rating >= minRating)
            return &squad[i];
    }
    return nullptr;
}

// ------------------------------------------------------------------
// [6] DEFAULT VALUES
//       rating = 50, winRate = 0, kd = 0, status = "scouting".
//       The caller only passes the three things that are always known.
// ------------------------------------------------------------------
player makePlayer(const string &name, const string &tag, const string &role,
                  int rating, float winRate, float kd, const string &status)
{
    player p;
    initPlayer(p, name, tag, role, rating, winRate, kd, status);
    return p;
}

void registerPlayer(player *&squad, int &count, int &capacity,
                    const string &name, const string &tag,
                    const string &role, int rating)
{
    if (findPlayer(squad, count, tag) != nullptr)
    {
        cout << "  [!] " << tag << " is already on the list\n";
        return;
    }

    player p = makePlayer(name, tag, role, rating);   // defaults used
    addPlayer(squad, count, capacity, p);
    cout << "  [+] " << tag << " added with " << rating << " rating\n";
}

// ------------------------------------------------------------------
// [7] SORTING ARRAYS OF STRUCTURES
// ------------------------------------------------------------------

// Selection sort, highest rating first.
// Selection sort is chosen (instead of a library sort) because the
// topic requires us to show the algorithm and because n is tiny.
void sortPlayersByRating(player squad[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        int best = i;
        for (int j = i + 1; j < count; j++)
        {
            if (squad[j].rating > squad[best].rating)
                best = j;
        }

        if (best != i)                        // swap whole structures
        {
            player temp = squad[i];
            squad[i]    = squad[best];
            squad[best] = temp;
        }
    }
}

// Bubble sort on the date text ("YYYY-MM-DD" sorts correctly as text).
void sortFixturesByDate(fixture fixtures[], int count)
{
    for (int pass = 0; pass < count - 1; pass++)
    {
        for (int i = 0; i < count - 1 - pass; i++)
        {
            if (fixtures[i].matchdate > fixtures[i + 1].matchdate)
            {
                fixture temp      = fixtures[i];
                fixtures[i]       = fixtures[i + 1];
                fixtures[i + 1]   = temp;
            }
        }
    }
}

// ------------------------------------------------------------------
// [8] SEARCHING ARRAYS OF STRUCTURES
// ------------------------------------------------------------------

// Linear search : works on an unsorted array, O(n).
player *searchByTag(player squad[], int count, const string &tag)
{
    for (int i = 0; i < count; i++)
    {
        if (squad[i].gamer_tag == tag)
            return &squad[i];
    }
    return nullptr;
}

// Binary search wrapper - the array MUST be sorted first (menu option 4
// does that before calling this).  Returns the array index or -1.
int searchByRating(player squad[], int count, int target)
{
    return binarySearchRating(squad, 0, count - 1, target);
}

// ------------------------------------------------------------------
// [9] MANIPULATING LINK LISTS OF STRUCTURES
//       The shortlist is a singly linked list of player copies.
//       A linked list is the right structure here because scouts add
//       and remove names in any order and we never need random access.
// ------------------------------------------------------------------

// Insert at the head - O(1), no matter how long the list is.
void shortlistAdd(ShortlistNode *&head, const player &p)
{
    if (shortlistContains(head, p.gamer_tag))
    {
        cout << "  [!] " << p.gamer_tag << " is already shortlisted\n";
        return;
    }

    ShortlistNode *node = new ShortlistNode;
    node->candidate = p;          // struct assignment copies every field
    node->next      = head;       // old head becomes second
    head            = node;       // new node is the head
    cout << "  [+] " << p.gamer_tag << " added to the shortlist\n";
}

// Remove by tag.  Returns false when the tag is not in the list.
bool shortlistRemove(ShortlistNode *&head, const string &tag)
{
    ShortlistNode *current = head;
    ShortlistNode *previous = nullptr;

    while (current != nullptr)
    {
        if (current->candidate.gamer_tag == tag)
        {
            if (previous == nullptr)          // removing the head
                head = current->next;
            else
                previous->next = current->next;

            delete current;                   // give the memory back
            return true;
        }
        previous = current;
        current  = current->next;
    }
    return false;
}

void shortlistPrint(const ShortlistNode *head)
{
    if (head == nullptr)
    {
        cout << "  (the shortlist is empty)\n";
        return;
    }

    const ShortlistNode *current = head;
    int position = 1;

    while (current != nullptr)
    {
        const player &p = current->candidate;
        cout << "  " << position << ". " << left << setw(16) << p.gamer_tag
             << " " << setw(12) << p.current_role
             << " rating " << p.rating
             << "  (" << p.scout_status << ")\n";
        current = current->next;
        position++;
    }
    cout << "  total : " << countShortlist(head) << " name(s) on the shortlist\n";
}

// Delete every node - without this the program leaks memory.
void shortlistClear(ShortlistNode *&head)
{
    while (head != nullptr)
    {
        ShortlistNode *next = head->next;
        delete head;
        head = next;
    }
}

bool shortlistContains(const ShortlistNode *head, const string &tag)
{
    const ShortlistNode *current = head;
    while (current != nullptr)
    {
        if (current->candidate.gamer_tag == tag)
            return true;
        current = current->next;
    }
    return false;
}
