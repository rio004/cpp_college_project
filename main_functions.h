/*
    Header file for all structures and function prototypes
    easier to use test functions
*/

#pragma once

#include <string>

using namespace std;

const int MAX_PLAYERS = 10;
const int MAX_FIXTURES = 10;

struct player
{
  int playerid;
  string real_name;
  string gamer_tag;
  string current_role;
  int rating;
  float winRate;
  float kdRatio;
  string scout_status;
};

struct game
{
  int gameid;
  string game_name;
};

struct fixture
{
  int fixture_id;
  string opponent;
  string matchdate;
  game match_stats;
};

// recursive
int findBestPlayerIndex(player team[], int n);

// sorting and overloads
void sortById(fixture fixtures[], int count);
void sortById(game games[], int count);

// Function Prototypes: FileI/O and Linear Search
void readingInputfiles(string filename, player Players[], int *p_numPlayers);
void writeToOutputFile(string filename, const player Players[], int numPlayers);

void writingToFixturesOutputFile(string filename, const fixture Fixtures[],
                                 int numFixtures);
void readingFixturesInputFiles(string filename, fixture Fixtures[],
                               int *p_numFixtures);

int searchForPlayerID(const player Players[], int size, int playerID);