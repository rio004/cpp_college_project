#include <fstream>
#include <iostream>
#include <sstream>

#include "main_functions.h"

using namespace std;

int findBestPlayerIndex(player team[], int n)
{
  if (n == 1)
  {
    return 0;
  }
  int bestIdx = findBestPlayerIndex(team, n - 1);
  if (team[n - 1].rating > team[bestIdx].rating)
  {
    return n - 1;
  }
  return bestIdx;
}

void sortById(fixture fixtures[], int count)
{
  // bubble sort
  bool swapped;

  for (int i = 0; i < count - 1; i++)
  {
    swapped = false;
    for (int j = 0; j < count - i - 1; j++)
    {
      // move the fixture id to the next index (swap)
      if (fixtures[j].fixture_id > fixtures[j + 1].fixture_id)
      {
        fixture tmp = fixtures[j];
        fixtures[j] = fixtures[j + 1];
        fixtures[j + 1] = tmp;
        swapped = true;
      }
    }

    if (!swapped) // already sorted
      break;
  }
}

void sortById(game games[], int count)
{
  // selection sort
  for (int i = 0; i < count - 1; i++)
  {
    int min_index = i;

    // Iterate through the unsorted portion
    // to find the actual minimum
    for (int j = i + 1; j < count; j++)
    {
      if (games[j].gameid < games[min_index].gameid)
      {
        min_index = j;
      }
    }

    // Move minimum element to its
    // correct position
    if (min_index != i)
    {
      game tmp = games[i];
      games[i] = games[min_index];
      games[min_index] = tmp;
    }
  }
}

void readingInputfiles(string filename, player Players[], int *p_numPlayers)
{
  ifstream inFile(filename); // Reads the file

  if (!inFile)
  {
    // Checks for errors incase the file won't open
    cerr << "Error opening file: " << filename << endl;
    return;
  }
  else
  {
    string line;
    getline(inFile, line); // Skips the hearder line texts
    *p_numPlayers = 0;

    // Reads the lines until the file ends or whent the array  is full
    while (getline(inFile, line) && *p_numPlayers < MAX_PLAYERS)
    {
      stringstream ss(line); // turns the rows into a stream so the getline
                             // function can start reading the file
      string hold;           // holds any number as text until it gets converted (int, floats)
      player list;           // creates a new list which is empty to fill in the different
                             // structs

      // The player's ID is delcared as a int so it reads it as text then
      // converts it
      getline(ss, hold, ',');
      list.playerid = stoi(hold); // the stoi function is what converts it into
                                  // a int from a string

      // These structs are just strings so it reads the text fields and stores
      // it straight into
      //  the struct which is indicated by each ','
      getline(ss, list.real_name, ',');
      getline(ss, list.gamer_tag, ',');
      getline(ss, list.current_role, ',');

      getline(ss, hold, ',');
      list.rating = stoi(hold);

      // The winrate element is an float because of the decimal point
      getline(ss, hold, ',');    // Reads it as a string
      list.winRate = stof(hold); // Then the 'stof' function converts it into a
                                 // float from a string

      getline(ss, hold, ',');
      list.kdRatio = stof(hold);

      getline(ss, list.scout_status, ',');

      Players[*p_numPlayers] = list; // Stores all the player data onto the next
                                     // free spot in the array
      (*p_numPlayers)++;
    }
    inFile.close(); // Closes the file
  }
}

void readingFixturesInputFiles(string filename, fixture Fixtures[],
                               int *p_numFixtures)
{
  ifstream inFile(filename);

  if (!inFile)
  {
    return;
  }
  else
  {
    string line;
    getline(inFile, line);
    *p_numFixtures = 0;

    while (getline(inFile, line) && *p_numFixtures < MAX_FIXTURES)
    {
      stringstream ss(line);
      string hold;
      fixture list;

      getline(ss, hold, ',');
      list.fixture_id = stoi(hold);

      getline(ss, list.opponent, ',');
      getline(ss, list.matchdate, ',');

      getline(ss, hold, ',');
      list.match_stats.gameid = stoi(hold);

      getline(ss, list.match_stats.game_name, ',');

      Fixtures[*p_numFixtures] = list;
      (*p_numFixtures)++;
    }
    inFile.close();
  }
}

void writeToOutputFile(string filename, const player Players[],
                       int numPlayers)
{
  ofstream outFile(filename);

  if (!outFile)
  {
    cerr << "Error writing to file: " << filename << endl;
    return;
  }

  outFile << "PlayerID,RealName,GamerTag,CurrentRole,Rating,WinRate,kdRatio,"
             "ScoutStatus\n"; // header row

  // Write one line per playlist, with commas inbetween the values
  for (int i = 0; i < numPlayers; i++)
  {
    outFile << Players[i].playerid << "," << Players[i].real_name << ","
            << Players[i].gamer_tag << "," << Players[i].current_role << ","
            << Players[i].rating << "," << Players[i].winRate << ","
            << Players[i].kdRatio << "," << Players[i].scout_status << "\n";
  }
  outFile.close();
  cout << "\nAll Availiable Player data are written to: " << filename << endl;
}

void writingToFixturesOutputFile(string filename, const fixture Fixtures[],
                                 int numFixtures)
{
  ofstream outFile(filename);

  if (!outFile)
  {
    cerr << "Error writing to file: " << filename << endl;
    return;
  }

  outFile << "FixtureID,Oppenent,MatchDate,gameid,gamename\n"; // header row

  for (int i = 0; i < numFixtures; i++)
  {
    outFile << Fixtures[i].fixture_id << ","
            << Fixtures[i].opponent << ","
            << Fixtures[i].matchdate << ","
            << Fixtures[i].match_stats.gameid << ","
            << Fixtures[i].match_stats.game_name << "\n";
  }
  outFile.close();
  cout << "\nAll Availiable Fixture data are written to: " << filename << endl;
}

// Linear Search: Searches the every element of the array and checks if the ID
// inputted by the user matches from the I/O files
int searchForPlayerID(const player players[], int size, int playerID)
{
  for (int i = 0; i < size; i++)
  { // Starts at index 0 of the array and starts
    // looking at all the elements
    if (players[i].playerid ==
        playerID)
    {           // Compares the players ID from the struct and the inputted
                // ID the user is looking for
      return i; // Stops the search when it finds the right match
    }
  }
  return -1;
}

Node *findCandidate(Node *head, const int &id)
{
  // Returns the node itself, not a copy, so the caller can edit it.
  Node *cur = head;

  while (cur != nullptr)
  {
    if (cur->data.playerid == id)
    {
      return cur;
    }
    cur = cur->next;
  }

  return nullptr;
}

bool addCandidate(Node *&head, const player &p)
{
  // Duplicate playerid are refused, otherwise one player gets two rows.
  if (findCandidate(head, p.playerid) != nullptr)
  {
    return false;
  }

  // New node goes to the front: one step, no other node moves.
  Node *node = new Node;
  node->data = p;
  node->next = head;
  head = node;

  return true;
}

bool popCandidate(Node *&head, player &out)
{
  if (head == nullptr)
  {
    return false;
  }

  // Copy the record out first, then unlink and free the node.
  out = head->data;

  Node *old = head;
  head = head->next;
  delete old;

  return true;
}

void clearList(Node *&head)
{
  // Take next before delete, otherwise the rest of the list is lost.
  while (head != nullptr)
  {
    Node *next = head->next;
    delete head;
    head = next;
  }
}

void showMenu()
{
  // Displays the main menu options to the user
  cout << "\n===== E-SPORTS SCOUTING MENU =====\n";
  cout << "1. List players\n";
  cout << "2. List fixtures\n";
  cout << "3. Find the best by rating\n";
  cout << "4. Sort by game\n";
  cout << "5. Sort by fixture\n";
  cout << "6. Shortlist\n";
  cout << "7. Exit\n";
  cout << "Enter your choice: ";
}

void listPlayers(const player Players[], int numPlayers)
{
  // Displays all players currently loaded in the Players array
  if (numPlayers <= 0)
  {
    cout << "No players available.\n";
    return;
  }

  for (int i = 0; i < numPlayers; i++)
  {
    cout << Players[i].playerid << " | " << Players[i].gamer_tag << " ("
         << Players[i].real_name << ") | Role: " << Players[i].current_role
         << " | Rating: " << Players[i].rating
         << " | WinRate: " << Players[i].winRate
         << " | K/D: " << Players[i].kdRatio
         << " | Status: " << Players[i].scout_status << endl;
  }
}

void listGames(const game games[], int numGames)
{
  // Displays all games currently loaded in the games array
  if (numGames <= 0)
  {
    cout << "No games available.\n";
    return;
  }

  for (int i = 0; i < numGames; i++)
  {
    cout << games[i].gameid << " | " << games[i].game_name << endl;
  }
}

void listFixtures(const fixture Fixtures[], int numFixtures)
{
  // Displays all fixtures currently loaded in the Fixtures array
  if (numFixtures <= 0)
  {
    cout << "No fixtures available.\n";
    return;
  }

  for (int i = 0; i < numFixtures; i++)
  {
    cout << Fixtures[i].fixture_id << " | vs " << Fixtures[i].opponent
         << " | Date: " << Fixtures[i].matchdate
         << " | Game: " << Fixtures[i].match_stats.game_name
         << " (ID: " << Fixtures[i].match_stats.gameid << ")" << endl;
  }
}

void runMenu(player Players[], int numPlayers, fixture Fixtures[],
             int numFixtures, game games[], int numGames)
{
  // Runs the main interactive menu loop and dispatches to project functions
  int choice = 0;

  while (choice != 7)
  {
    showMenu();
    if (!(cin >> choice))
    {
      cout << "Invalid input. Please enter a number.\n";
      continue;
    }

    switch (choice)
    {
    case 1:
      // List players
      listPlayers(Players, numPlayers);
      break;

    case 2:
      // List fixtures
      listFixtures(Fixtures, numFixtures);
      break;

    case 3:
      // Find the best by rating
      if (numPlayers > 0)
      {
        int bestIdx = findBestPlayerIndex(Players, numPlayers);
        cout << "Best player: " << Players[bestIdx].gamer_tag
             << " (" << Players[bestIdx].real_name << ") - Rating: "
             << Players[bestIdx].rating << endl;
      }
      else
      {
        cout << "No players available.\n";
      }
      break;

    case 4:
      // Sort by game
      if (numGames > 0)
      {
        sortById(games, numGames);
        cout << "Games sorted by ID:\n";
        listGames(games, numGames);
      }
      else
      {
        cout << "No games available.\n";
      }
      break;

    case 5:
      // Sort by fixture
      if (numFixtures > 0)
      {
        sortById(Fixtures, numFixtures);
        cout << "Fixtures sorted by ID:\n";
        listFixtures(Fixtures, numFixtures);
      }
      else
      {
        cout << "No fixtures available.\n";
      }
      break;

    case 6:
      // Shortlist
      cout << "Shortlist\n";
      break;

    case 7:
      cout << "Exiting menu.\n";
      break;

    default:
      cout << "Invalid choice. Please select 1-7.\n";
      break;
    }
  }
}