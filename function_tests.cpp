/*
    Unit tests for the shortlist (the linked list of players)
*/

#include "main_functions.h"

#include <gtest/gtest.h>

static player makePlayer(int id, const string &tag, const string &status)
{
    player p;
    p.playerid = id;
    p.real_name = tag + " Real";
    p.gamer_tag = tag;
    p.current_role = "Mid";
    p.rating = 80;
    p.winRate = 50.0f;
    p.kdRatio = 1.0f;
    p.scout_status = status;

    return p;
}

TEST(ShortlistTest, AddPutsTheNewNodeAtTheFront)
{
    Node *head = nullptr;

    addCandidate(head, makePlayer(1, "First", "Unsigned"));
    addCandidate(head, makePlayer(2, "Second", "Unsigned"));

    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->data.playerid, 2);
    EXPECT_EQ(head->next->data.playerid, 1);

    clearList(head);
    EXPECT_EQ(head, nullptr);
}

TEST(ShortlistTest, SameIdTwiceIsRefused)
{
    Node *head = nullptr;

    EXPECT_TRUE(addCandidate(head, makePlayer(7, "Faker", "Unsigned")));
    EXPECT_FALSE(addCandidate(head, makePlayer(7, "Faker", "Unsigned")));

    EXPECT_EQ(head->next, nullptr);

    clearList(head);
}

TEST(ShortlistTest, PopReturnsTheFirstCandidate)
{
    Node *head = nullptr;
    player popped;

    EXPECT_FALSE(popCandidate(head, popped));

    addCandidate(head, makePlayer(1, "First", "Unsigned"));
    addCandidate(head, makePlayer(2, "Second", "Unsigned"));

    ASSERT_TRUE(popCandidate(head, popped));
    EXPECT_EQ(popped.playerid, 2);
    EXPECT_EQ(head->data.playerid, 1);

    clearList(head);
}

TEST(ShortlistTest, RemoveWorksForHeadMiddleAndMissing)
{
    Node *head = nullptr;

    addCandidate(head, makePlayer(1, "First", "Unsigned"));
    addCandidate(head, makePlayer(2, "Second", "Unsigned"));
    addCandidate(head, makePlayer(3, "Third", "Unsigned"));

    EXPECT_TRUE(removeCandidate(head, 2));
    EXPECT_EQ(head->data.playerid, 3);
    EXPECT_EQ(head->next->data.playerid, 1);

    EXPECT_TRUE(removeCandidate(head, 3));
    EXPECT_EQ(head->data.playerid, 1);

    EXPECT_FALSE(removeCandidate(head, 99));

    clearList(head);
}
