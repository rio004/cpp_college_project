/*
    Header file for all structures and function prototypes
    easier to use test functions
*/

#pragma once

#include <string>

using namespace std;

struct player {
  string real_name;
  string gamer_tag;
  string current_role;
  int rating;
  float winRate;
  float kdRatio;
  string scout_status;
};

struct game {
  int gameid;
  string game_name;
};

struct fixture {
  int fixture_id;
  string opponent;
  string matchdate;
  game match_stats;
};

int findBestPlayerIndex(player team[], int n);