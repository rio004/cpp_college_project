#pragma once

#include <iostream>
#include <string>

using namespace std;

struct player
{
    string real_name;
    string gamer_tag;
    string current_role;
    int    rating;
    float  winRate;
    float  kdRatio;
    string scout_status;
};

void show(const player &p);
void show(const player &p, int position);

player *findPlayer(player squad[], int count, const string &tag);
player *findPlayer(player squad[], int count, int rating);

player makePlayer(const string &name, const string &tag, const string &role,
                  int rating = 50, float winRate = 0.0f, float kdRatio = 0.0f,
                  const string &status = "scouting");
