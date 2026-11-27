#pragma once

#include <iostream>
#include <string>

using namespace std;

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

struct Node
{
    player data;
    Node  *next;
};

void  initList(Node *&head);
bool  addCandidate(Node *&head, const player &p);
bool  popCandidate(Node *&head, player &out);
bool  removeCandidate(Node *&head, const string &tag);
Node *findCandidate(Node *head, const string &tag);
void  showCandidate(const player &p);
void  printList(const Node *head);
int   countCandidates(const Node *head);
void  clearList(Node *&head);
