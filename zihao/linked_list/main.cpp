#include "linked_list.h"

static player makePlayer(const string &name, const string &tag, const string &role,
                         int rating, float winRate, float kd, const string &status)
{
    player p;
    p.real_name    = name;
    p.gamer_tag    = tag;
    p.current_role = role;
    p.rating       = rating;
    p.winRate      = winRate;
    p.kdRatio      = kd;
    p.scout_status = status;
    return p;
}

int main()
{
    Node *shortlist = nullptr;
    initList(shortlist);

    cout << "=== 1. empty list ===" << endl;
    printList(shortlist);
    cout << "  count = " << countCandidates(shortlist) << endl;
    cout << endl;

    cout << "=== 2. add four candidates ===" << endl;
    addCandidate(shortlist, makePlayer("Aaron Byrne", "Vortex", "Duelist", 88, 64.5f, 1.32f, "shortlisted"));
    addCandidate(shortlist, makePlayer("Daniel Walsh", "Nyx", "Controller", 79, 58.0f, 1.05f, "scouting"));
    addCandidate(shortlist, makePlayer("Sofia Martins", "Kestrel", "Initiator", 91, 71.2f, 1.44f, "signed"));
    addCandidate(shortlist, makePlayer("Tom Reid", "Echo", "Sentinel", 84, 61.8f, 1.18f, "scouting"));
    printList(shortlist);
    cout << "  count = " << countCandidates(shortlist) << endl;
    cout << endl;

    cout << "=== 3. a duplicate is refused ===" << endl;
    if (addCandidate(shortlist, makePlayer("Aaron Byrne", "Vortex", "Duelist", 88, 64.5f, 1.32f, "shortlisted")))
    {
        cout << "  Vortex added again" << endl;
    }
    else
    {
        cout << "  Vortex is already on the list, not added again" << endl;
    }
    cout << endl;

    cout << "=== 4. find one candidate by tag ===" << endl;
    Node *found = findCandidate(shortlist, "Nyx");
    if (found != nullptr)
    {
        cout << "  found " << found->data.gamer_tag
             << " (" << found->data.real_name << ", rating "
             << found->data.rating << ")" << endl;
    }
    else
    {
        cout << "  no candidate with that tag" << endl;
    }
    cout << endl;

    cout << "=== 5. remove the FIRST candidate (Echo) ===" << endl;
    if (removeCandidate(shortlist, "Echo"))
    {
        cout << "  Echo removed, head moved to the next node" << endl;
    }
    printList(shortlist);
    cout << endl;

    cout << "=== 6. remove a candidate from the MIDDLE (Kestrel) ===" << endl;
    if (removeCandidate(shortlist, "Kestrel"))
    {
        cout << "  Kestrel removed, the node before it now points past it" << endl;
    }
    printList(shortlist);
    cout << "  count = " << countCandidates(shortlist) << endl;
    cout << endl;

    cout << "=== 7. remove a tag that is not there ===" << endl;
    if (removeCandidate(shortlist, "Ghost"))
    {
        cout << "  Ghost removed" << endl;
    }
    else
    {
        cout << "  Ghost was not on the list, nothing changed" << endl;
    }
    printList(shortlist);
    cout << endl;

    cout << "=== 8. free the whole list ===" << endl;
    clearList(shortlist);
    printList(shortlist);
    cout << "  count = " << countCandidates(shortlist) << endl;

    return 0;
}
