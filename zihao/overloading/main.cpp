#include "overloads.h"

int main()
{
    player squad[MAX_PLAYERS];
    int count = 0;

    cout << "========== OVERLOADING AND DEFAULT VALUES ==========" << endl;
    cout << endl;

    cout << "[1] three players, all fields given" << endl;
    squad[count++] = makePlayer(7, "Lee Sang-hyeok", "Faker", "Mid", 96, 67.4f, 4.10f, "Signed");
    squad[count++] = makePlayer(45, "Choi Woo-je", "Zeus", "Top", 92, 65.2f, 3.42f, "Unsigned");
    squad[count++] = makePlayer(89, "Moon Hyeon-jun", "Oner", "Jungle", 93, 61.8f, 4.65f, "Unsigned");

    cout << "[2] one player with the default values" << endl;
    squad[count++] = makePlayer(120, "New Player", "Rookie", "Support");
    cout << endl;

    cout << "[3] show(player) - the full record" << endl;
    show(squad[0]);
    cout << endl;

    cout << "[4] show(player, index) - one line of the list" << endl;
    for (int i = 0; i < count; i++)
    {
        show(squad[i], i);
    }
    cout << endl;

    cout << "[5] findPlayer(tag) - string argument" << endl;
    int pos = findPlayer(squad, count, "Zeus");
    if (pos >= 0)
    {
        show(squad[pos]);
    }
    else
    {
        cout << "  Zeus not found" << endl;
    }
    cout << endl;

    cout << "[6] findPlayer(id) - int argument, same name" << endl;
    pos = findPlayer(squad, count, 89);
    if (pos >= 0)
    {
        show(squad[pos]);
    }
    else
    {
        cout << "  89 not found" << endl;
    }
    cout << endl;

    cout << "[7] a tag and an id that are not there" << endl;
    cout << "  findPlayer(tag 'Nyx')   -> " << findPlayer(squad, count, "Nyx") << endl;
    cout << "  findPlayer(id 999)      -> " << findPlayer(squad, count, 999) << endl;

    return 0;
}
