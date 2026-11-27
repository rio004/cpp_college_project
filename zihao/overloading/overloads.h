#pragma once

#include <iostream>
#include <string>

using namespace std;

const int MAX_PLAYERS = 10;

struct player
{
    int    playerid;
    string real_name;
    string gamer_tag;
    string current_role;
    int    rating;
    float  winRate;
    float  kdRatio;
    string scout_status;
};

player makePlayer(int playerid, const string &name, const string &tag, const string &role,
                  int rating = 50, float winRate = 0.0f, float kdRatio = 0.0f,
                  const string &status = "Unsigned");

void show(const player &p);
void show(const player &p, int index);

int findPlayer(player players[], int count, const string &tag);
int findPlayer(player players[], int count, int playerid);
