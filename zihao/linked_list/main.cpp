#include "linked_list.h"

int main()
{
    Node *shortlist;
    initList(shortlist);

    cout << "================ SHORTLIST (LINKED LIST) ================" << endl;
    cout << endl;

    cout << "[1] the new shortlist is empty" << endl;
    printList(shortlist);
    cout << "    count = " << countCandidates(shortlist) << endl;
    cout << endl;

    player p[4];
    p[0].playerid = 7;  p[0].real_name = "Lee Sang-hyeok"; p[0].gamer_tag = "Faker";
    p[0].current_role = "Mid";     p[0].rating = 96; p[0].winRate = 67.4f; p[0].kdRatio = 4.10f;
    p[0].scout_status = "Signed";

    p[1].playerid = 6;  p[1].real_name = "Ryu Min-seok";   p[1].gamer_tag = "Keria";
    p[1].current_role = "Support"; p[1].rating = 94; p[1].winRate = 66.1f; p[1].kdRatio = 3.85f;
    p[1].scout_status = "Signed";

    p[2].playerid = 45; p[2].real_name = "Choi Woo-je";    p[2].gamer_tag = "Zeus";
    p[2].current_role = "Top";     p[2].rating = 92; p[2].winRate = 65.2f; p[2].kdRatio = 3.42f;
    p[2].scout_status = "Unsigned";

    p[3].playerid = 89; p[3].real_name = "Moon Hyeon-jun"; p[3].gamer_tag = "Oner";
    p[3].current_role = "Jungle";  p[3].rating = 93; p[3].winRate = 61.8f; p[3].kdRatio = 4.65f;
    p[3].scout_status = "Unsigned";

    cout << "[2] add four candidates (each one goes to the front)" << endl;
    for (int i = 0; i < 4; i++)
    {
        addCandidate(shortlist, p[i]);
    }
    printList(shortlist);
    cout << "    count = " << countCandidates(shortlist) << endl;
    cout << endl;

    cout << "[3] add Faker again -> refused, the tag is already there" << endl;
    if (!addCandidate(shortlist, p[0]))
    {
        cout << "    Faker is already on the shortlist" << endl;
    }
    cout << "    count = " << countCandidates(shortlist) << endl;
    cout << endl;

    cout << "[4] find Zeus by tag -> pointer returned, no copy" << endl;
    Node *found = findCandidate(shortlist, "Zeus");
    if (found != nullptr)
    {
        showCandidate(found->data);
    }
    else
    {
        cout << "    Zeus not found" << endl;
    }
    cout << endl;

    cout << "[5] pop the first candidate -> record printed, node deleted" << endl;
    player popped;
    if (popCandidate(shortlist, popped))
    {
        cout << "    popped: " << popped.gamer_tag << endl;
        showCandidate(popped);
    }
    else
    {
        cout << "    nothing to pop" << endl;
    }
    printList(shortlist);
    cout << endl;

    cout << "[6] remove Keria from the middle" << endl;
    removeCandidate(shortlist, "Keria");
    printList(shortlist);
    cout << endl;

    cout << "[7] remove a tag that is not there" << endl;
    if (!removeCandidate(shortlist, "Nyx"))
    {
        cout << "    Nyx is not on the shortlist" << endl;
    }
    cout << endl;

    cout << "[8] clear the whole list" << endl;
    clearList(shortlist);
    printList(shortlist);
    cout << "    count = " << countCandidates(shortlist) << endl;

    return 0;
}
