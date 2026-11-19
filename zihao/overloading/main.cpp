#include "overloads.h"

int main()
{
    player squad[6];
    int count = 0;

    cout << "=== 1. build the squad ===" << endl;
    squad[count++] = makePlayer("Aaron Byrne", "Vortex", "Duelist", 88, 64.5f, 1.32f, "shortlisted");
    squad[count++] = makePlayer("Daniel Walsh", "Nyx", "Controller", 79);
    squad[count++] = makePlayer("Sofia Martins", "Kestrel", "Initiator");
    squad[count++] = makePlayer("Tom Reid", "Echo", "Sentinel", 84, 61.8f, 1.18f, "scouting");

    for (int i = 0; i < count; i++)
    {
        show(squad[i], i + 1);
    }
    cout << endl;

    cout << "=== 2. the defaults did their job ===" << endl;
    cout << "  Nyx     rating " << squad[1].rating
         << ", win rate " << squad[1].winRate
         << ", K/D " << squad[1].kdRatio
         << ", status " << squad[1].scout_status << endl;
    cout << "  Kestrel rating " << squad[2].rating
         << " (no rating was passed in)" << endl;
    cout << endl;

    cout << "=== 3. show one player in detail ===" << endl;
    show(squad[0]);
    cout << endl;

    cout << "=== 4. findPlayer by tag (third argument is a string) ===" << endl;
    player *found = findPlayer(squad, count, "Nyx");
    if (found != nullptr)
    {
        cout << "  found " << found->gamer_tag << " (" << found->real_name << ")" << endl;
    }
    else
    {
        cout << "  not found" << endl;
    }
    cout << endl;

    cout << "=== 5. findPlayer by rating (third argument is an int) ===" << endl;
    found = findPlayer(squad, count, 88);
    if (found != nullptr)
    {
        cout << "  rating 88 belongs to " << found->gamer_tag << endl;
    }
    else
    {
        cout << "  no player with that rating" << endl;
    }
    cout << endl;

    cout << "=== 6. a tag that is not there ===" << endl;
    found = findPlayer(squad, count, "Ghost");
    if (found == nullptr)
    {
        cout << "  Ghost is not in the squad" << endl;
    }
    cout << endl;

    cout << "=== 7. same call shape, different function ===" << endl;
    cout << "  findPlayer(squad, count, \"Nyx\") -> the string version" << endl;
    cout << "  findPlayer(squad, count, 88)    -> the int version" << endl;
    cout << "  the compiler decides from the type of the third argument" << endl;

    return 0;
}
