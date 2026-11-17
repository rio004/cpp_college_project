/*
    main.cpp
    ------------------------------------------------------------------
    Software Development 1 - Group Assignment (Group 4)
    Esports Scouting & Recruitment System

    This file is the SCREEN + MENU layer (my part of the group work).
    It owns the console layout, the menu loop and the input handling.
    All the real work is done by the functions in main_functions.cpp.

    Written by : Qi Zihao  (X00218059)
*/

#include "main_functions.h"

#include <fstream>
#include <limits>
#include <string>

// ------------------------------------------------------------------
// Screen / menu constants
// ------------------------------------------------------------------
const string APP_TITLE = "ESPORTS SCOUTING & RECRUITMENT SYSTEM";
const string DATA_DIR  = "data/";

// ------------------------------------------------------------------
// Small input helpers - kept here so main() stays readable.
// ------------------------------------------------------------------

// Throw away the rest of the line after "cin >> number".
void clearInputLine()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Read a whole line (names and tags contain spaces, so >> is not enough).
string readLine(const string &prompt)
{
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// Read one number and drop the newline.
int readNumber(const string &prompt)
{
    int value = 0;
    cout << prompt;
    if (!(cin >> value))
    {
        cin.clear();
        clearInputLine();
        return -1;
    }
    clearInputLine();
    return value;
}

// ------------------------------------------------------------------
// Screen drawing  ([0A] menu and navigation of the report)
// ------------------------------------------------------------------

// The banner is printed at the top of every screen so the user always
// knows which application they are inside.
void printBanner()
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "   " << APP_TITLE << "\n";
    cout << "   Group 4  -  scouting console v1.0\n";
    cout << "====================================================\n";
}

void printMainMenu()
{
    cout << "\n  MAIN MENU\n";
    printDivider();
    cout << "  1. Show the squad                 (array of structures)\n";
    cout << "  2. Register a new player          (default values)\n";
    cout << "  3. Log a match result             (pointer / reference)\n";
    cout << "  4. Sort the squad by rating       (selection sort)\n";
    cout << "  5. Find a player by tag           (linear search)\n";
    cout << "  6. Find a player by rating        (binary search)\n";
    cout << "  7. Shortlist manager              (linked list)\n";
    cout << "  8. Fixtures                       (nested structures)\n";
    cout << "  9. Save the squad to file         (file output)\n";
    cout << " 10. Reload the squad from file     (file input)\n";
    cout << "  0. Exit\n";
    printDivider();
}

// ------------------------------------------------------------------
// Menu actions
// ------------------------------------------------------------------

// Option 1 : show every player (array of structures passed to a function)
void showSquadScreen(player squad[], int count)
{
    printBanner();
    cout << "\n  SQUAD / SCOUTING LIST\n";
    printDivider();

    if (count == 0)
        cout << "  (no players yet)\n";
    else
        printSquad(squad, count);      // [2.2] array of structures

    cout << "\n  \"*\" marks a star candidate (rating >= 85, K/D >= 1.2,"
            " win rate >= 60)\n";
}

// Option 2 : register a player, demonstrating DEFAULT VALUES
void registerPlayerScreen(player *&squad, int &count, int &capacity)
{
    printBanner();
    cout << "\n  REGISTER A NEW PLAYER\n";
    printDivider();

    string name = readLine("  Real name        : ");
    string tag  = readLine("  Gamer tag        : ");
    string role = readLine("  Role             : ");

    cout << "  Rating 0-100 (press Enter to use the default of 50)\n";
    string ratingText = readLine("  Rating           : ");
    int rating = DEFAULT_RATING;                    // default value
    if (!ratingText.empty())
        rating = atoi(ratingText.c_str());

    registerPlayer(squad, count, capacity, name, tag, role, rating);
}

// Option 3 : log one map result.
// The player is found with a search, changed through a pointer and a
// reference so the real record inside the array is updated.
void logMatchScreen(player *&squad, int &count, int &capacity)
{
    printBanner();
    cout << "\n  LOG A MATCH RESULT\n";
    printDivider();

    string tag = readLine("  Gamer tag        : ");
    player *p = findPlayer(squad, count, tag);      // overload, by tag

    if (p == nullptr)
    {
        cout << "  [!] no player called " << tag << "\n";
        return;
    }

    int kills  = readNumber("  Kills            : ");
    int deaths = readNumber("  Deaths           : ");

    applyMatchResult(*p, kills, deaths);            // [2.4] reference
    writeLog("map result logged for " + tag);

    cout << "\n  updated record\n";
    printDivider();
    show(*p);                                       // overload, by player
}

// Option 4 : sort and show the squad
void sortSquadScreen(player squad[], int count)
{
    printBanner();
    cout << "\n  SORT THE SQUAD BY RATING (highest first)\n";
    printDivider();

    if (count < 2)
    {
        cout << "  (nothing to sort)\n";
        return;
    }

    sortPlayersByRating(squad, count);              // [7] selection sort
    printSquad(squad, count);
}

// Option 5 : linear search by tag
void findPlayerScreen(player squad[], int count)
{
    printBanner();
    cout << "\n  FIND A PLAYER BY TAG (linear search)\n";
    printDivider();

    string tag = readLine("  Gamer tag        : ");
    player *p = searchByTag(squad, count, tag);     // [8] linear search

    if (p == nullptr)
        cout << "  [!] " << tag << " is not on the scouting list\n";
    else
    {
        cout << "\n  found\n";
        printDivider();
        show(*p);
    }
}

// Option 6 : binary search by rating.
// The array has to be sorted first, so the menu sorts a copy of the
// index list by calling the sort function before the search.
void findByRatingScreen(player squad[], int count)
{
    printBanner();
    cout << "\n  FIND A PLAYER BY RATING (binary search)\n";
    printDivider();

    if (count == 0)
    {
        cout << "  (no players yet)\n";
        return;
    }

    int target = readNumber("  Rating to look for : ");

    sortPlayersByRating(squad, count);              // binary search needs order
    cout << "  (the squad was sorted first, this is required by binary search)\n";

    int index = searchByRating(squad, count, target);

    if (index == -1)
        cout << "  [!] nobody with rating exactly " << target << "\n";
    else
        show(squad[index]);
}

// Option 7 : the shortlist (singly linked list)
void shortlistScreen(player squad[], int count, ShortlistNode *&head)
{
    int choice = -1;
    printBanner();
    while (choice != 0)
    {
        cout << "\n  SHORTLIST MANAGER (linked list)\n";
        printDivider();
        cout << "  1. Add a player by tag\n";
        cout << "  2. Show the shortlist\n";
        cout << "  3. Remove a player by tag\n";
        cout << "  0. Back to the main menu\n";
        printDivider();

        choice = readNumber("  choice: ");

        if (choice == 1)
        {
            string tag = readLine("  Gamer tag        : ");
            player *p = findPlayer(squad, count, tag);
            if (p == nullptr)
                cout << "  [!] " << tag << " is not on the scouting list\n";
            else
            {
                shortlistAdd(head, *p);            // [9] head insert
                writeLog("shortlisted " + tag);
            }
        }
        else if (choice == 2)
        {
            shortlistPrint(head);
        }
        else if (choice == 3)
        {
            string tag = readLine("  Gamer tag        : ");
            if (shortlistRemove(head, tag))
                cout << "  [-] " << tag << " removed from the shortlist\n";
            else
                cout << "  [!] " << tag << " was not on the shortlist\n";
        }
    }
}

// Option 8 : fixtures (array of structures that contains a structure)
void fixturesScreen(fixture fixtures[], int fixtureCount)
{
    int choice = -1;
    printBanner();
    while (choice != 0)
    {
        cout << "\n  FIXTURES\n";
        printDivider();
        cout << "  1. Show the fixtures\n";
        cout << "  2. Sort the fixtures by date\n";
        cout << "  3. Total kills this season (recursion)\n";
        cout << "  4. Show one fixture in detail\n";
        cout << "  0. Back to the main menu\n";
        printDivider();

        choice = readNumber("  choice: ");

        if (choice == 1)
        {
            if (fixtureCount == 0)
                cout << "  (no fixtures loaded)\n";
            else
                printFixtures(fixtures, fixtureCount);
        }
        else if (choice == 2)
        {
            sortFixturesByDate(fixtures, fixtureCount);   // [7] bubble sort
            cout << "  sorted by date\n";
            printFixtures(fixtures, fixtureCount);
        }
        else if (choice == 3)
        {
            cout << "  total kills : "
                 << totalKills(fixtures, fixtureCount) << "  (counted recursively)\n";
        }
        else if (choice == 4)
        {
            int id = readNumber("  fixture id       : ");
            bool found = false;
            for (int i = 0; i < fixtureCount && !found; i++)
            {
                if (fixtures[i].fixture_id == id)
                {
                    show(fixtures[i]);                    // overload, by fixture
                    found = true;
                }
            }
            if (!found)
                cout << "  [!] no fixture with that id\n";
        }
    }
}

// ------------------------------------------------------------------
// main - the menu loop
// ------------------------------------------------------------------
int main()
{
    // --- dynamic array of players (needs new / delete) -------------
    int capacity = 4;                              // starts small on purpose
    int count    = 0;
    player *squad = new player[capacity];

    fixture fixtures[MAX_FIXTURES];
    int fixtureCount = 0;

    ShortlistNode *shortlist = nullptr;            // empty linked list

    // --- initial data comes from a file ----------------------------
    printBanner();
    cout << "\n  loading data ...\n";
    int loaded = loadPlayers(squad, count, capacity, DATA_DIR + "players.txt");
    cout << "  " << loaded << " player(s) read from " << DATA_DIR << "players.txt\n";

    fixtureCount = loadFixtures(fixtures, MAX_FIXTURES,
                                DATA_DIR + "fixtures.txt", squad, count);
    cout << "  " << fixtureCount << " fixture(s) read from " << DATA_DIR
         << "fixtures.txt\n";

    bool running = true;

    while (running)
    {
        printMainMenu();
        int choice = readNumber("  choice: ");

        if (cin.eof())                              // input stream closed
            break;

        switch (choice)
        {
            case 1:
                showSquadScreen(squad, count);
                break;

            case 2:
                registerPlayerScreen(squad, count, capacity);
                // the squad array may have moved, so the MVP pointers
                // inside the fixtures have to be rebuilt
                relinkFixtures(fixtures, fixtureCount, squad, count);
                break;

            case 3:
                logMatchScreen(squad, count, capacity);
                break;

            case 4:
                sortSquadScreen(squad, count);
                // sorting moves players around inside the array, so the
                // MVP pointers have to be refreshed as well
                relinkFixtures(fixtures, fixtureCount, squad, count);
                break;

            case 5:
                findPlayerScreen(squad, count);
                break;

            case 6:
                findByRatingScreen(squad, count);
                break;

            case 7:
                shortlistScreen(squad, count, shortlist);
                break;

            case 8:
                fixturesScreen(fixtures, fixtureCount);
                break;

            case 9:
                savePlayers(squad, count, DATA_DIR + "players_out.txt");
                break;

            case 10:
                // reload from the original file, starting from scratch
                freeSquad(squad);
                capacity = 4;
                count    = 0;
                squad    = new player[capacity];
                loaded   = loadPlayers(squad, count, capacity,
                                       DATA_DIR + "players.txt");
                cout << "  " << loaded << " player(s) reloaded\n";
                break;

            case 0:
                running = false;
                break;

            default:
                cout << "  [!] " << choice << " is not a menu option\n";
                break;
        }
    }

    // --- clean up before leaving -----------------------------------
    cout << "\n  saving the squad before exit ...\n";
    savePlayers(squad, count, DATA_DIR + "players_out.txt");

    shortlistClear(shortlist);                      // free every node
    freeSquad(squad);                               // free the array

    cout << "\n  Thank you for using the scouting console.\n";
    cout << "  Press Enter to close.\n";
    cout.flush();

    return 0;
}
