#include "overloads.h"

#include <iomanip>

player makePlayer(int playerid, const string &name, const string &tag, const string &role,
                  int rating, float winRate, float kdRatio, const string &status)
{
    player p;
    p.playerid = playerid;
    p.real_name = name;
    p.gamer_tag = tag;
    p.current_role = role;
    p.rating = rating;
    p.winRate = winRate;
    p.kdRatio = kdRatio;
    p.scout_status = status;

    return p;
}

void show(const player &p)
{
    cout << "  ID        : " << p.playerid << endl;
    cout << "  Name      : " << p.real_name << endl;
    cout << "  Gamer tag : " << p.gamer_tag << endl;
    cout << "  Role      : " << p.current_role << endl;
    cout << "  Rating    : " << p.rating << endl;
    cout << "  Win rate  : " << fixed << setprecision(1) << p.winRate << " %" << endl;
    cout << "  K/D ratio : " << setprecision(2) << p.kdRatio << endl;
    cout.unsetf(ios::fixed);
    cout.precision(6);
    cout << "  Status    : " << p.scout_status << endl;
}

void show(const player &p, int index)
{
    cout << "  [" << index << "] " << p.playerid;

    int gap = 4 - (int)to_string(p.playerid).size();
    for (int i = 0; i < gap; i++)
    {
        cout << " ";
    }

    cout << p.gamer_tag;

    gap = 12 - (int)p.gamer_tag.size();
    for (int i = 0; i < gap; i++)
    {
        cout << " ";
    }

    cout << p.current_role;

    gap = 12 - (int)p.current_role.size();
    for (int i = 0; i < gap; i++)
    {
        cout << " ";
    }

    cout << p.rating << "  " << p.scout_status << endl;
}

int findPlayer(player players[], int count, const string &tag)
{
    for (int i = 0; i < count; i++)
    {
        if (players[i].gamer_tag == tag)
        {
            return i;
        }
    }

    return -1;
}

int findPlayer(player players[], int count, int playerid)
{
    for (int i = 0; i < count; i++)
    {
        if (players[i].playerid == playerid)
        {
            return i;
        }
    }

    return -1;
}
