#include "linked_list.h"

void initList(Node *&head)
{
    head = nullptr;
}

bool addCandidate(Node *&head, const player &p)
{
    if (findCandidate(head, p.gamer_tag) != nullptr)
    {
        return false;
    }

    Node *node = new Node;
    node->data = p;
    node->next = head;
    head = node;

    return true;
}

bool removeCandidate(Node *&head, const string &tag)
{
    Node *prev = nullptr;
    Node *cur = head;

    while (cur != nullptr)
    {
        if (cur->data.gamer_tag == tag)
        {
            if (prev == nullptr)
            {
                head = cur->next;
            }
            else
            {
                prev->next = cur->next;
            }

            delete cur;
            return true;
        }

        prev = cur;
        cur = cur->next;
    }

    return false;
}

Node *findCandidate(Node *head, const string &tag)
{
    Node *cur = head;

    while (cur != nullptr)
    {
        if (cur->data.gamer_tag == tag)
        {
            return cur;
        }
        cur = cur->next;
    }

    return nullptr;
}

void printList(const Node *head)
{
    if (head == nullptr)
    {
        cout << "  (the shortlist is empty)" << endl;
        return;
    }

    const Node *cur = head;
    int position = 1;

    cout << "  No  Gamer tag     Role          Rating  Status" << endl;
    cout << "  --  ------------  ------------  ------  -----------" << endl;

    while (cur != nullptr)
    {
        cout << "  " << position << "   " << cur->data.gamer_tag;
        for (int i = (int)cur->data.gamer_tag.size(); i < 12; i++)
        {
            cout << " ";
        }
        cout << "  " << cur->data.current_role;
        for (int i = (int)cur->data.current_role.size(); i < 12; i++)
        {
            cout << " ";
        }
        cout << "  " << cur->data.rating << "      " << cur->data.scout_status << endl;

        cur = cur->next;
        position++;
    }
}

int countCandidates(const Node *head)
{
    int count = 0;
    const Node *cur = head;

    while (cur != nullptr)
    {
        count++;
        cur = cur->next;
    }

    return count;
}

void clearList(Node *&head)
{
    while (head != nullptr)
    {
        Node *next = head->next;
        delete head;
        head = next;
    }
}
