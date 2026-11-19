#include "overloads.h"

void show(const player &p)
{
    cout << "  Name      : " << p.real_name << endl;
    cout << "  Tag       : " << p.gamer_tag << endl;
    cout << "  Role      : " << p.current_role << endl;
    cout << "  Rating    : " << p.rating << endl;
    cout << "  Status    : " << p.scout_status << endl;
}

void show(const player &p, int position)
{
    cout << "  " << position << "   " << p.gamer_tag
         << "  (" << p.current_role << ", rating " << p.rating << ")" << endl;
}

player *findPlayer(player squad[], int count, const string &tag)
{
    for (int i = 0; i < count; i++)
    {
        if (squad[i].gamer_tag == tag)
        {
            return &squad[i];
        }
    }
    return nullptr;
}

player *findPlayer(player squad[], int count, int rating)
{
    for (int i = 0; i < count; i++)
    {
        if (squad[i].rating == rating)
        {
            return &squad[i];
        }
    }
    return nullptr;
}

player makePlayer(const string &name, const string &tag, const string &role,
                  int rating, float winRate, float kdRatio, const string &status)
{
    player p;
    p.real_name    = name;
    p.gamer_tag    = tag;
    p.current_role = role;
    p.rating       = rating;
    p.winRate      = winRate;
    p.kdRatio      = kdRatio;
    p.scout_status = status;
    return p;
}
