/*
    function_tests.cpp
    ------------------------------------------------------------------
    Unit tests for the scouting system (Topic 11).

    Testing method used : Boundary Value Analysis (BVA) on the star
    candidate rule, plus a few equivalence-partition checks on the
    search and the linked list.  Every test follows the AAA layout:

        Arrange  - build the input data
        Act      - call the function under test once
        Assert   - check the result

    Built with Google Test (gtest).
*/

#include "main_functions.h"

#include <gtest/gtest.h>

// ------------------------------------------------------------------
// Helper : build a small squad for the tests (same data every run,
// so a failing test is repeatable).
// ------------------------------------------------------------------
static void fillTestSquad(player squad[], int &count)
{
    initPlayer(squad[0], "Aaron Byrne",  "Vortex",  "Duelist",    88, 64.5f, 1.32f, "shortlisted");
    initPlayer(squad[1], "Daniel Walsh", "Nyx",     "Controller", 79, 58.0f, 1.05f, "scouting");
    initPlayer(squad[2], "Sofia Martins","Kestrel", "Initiator",  91, 71.2f, 1.44f, "signed");
    initPlayer(squad[3], "Liam Doyle",   "Havoc",   "Duelist",    73, 52.4f, 0.98f, "scouting");
    count = 4;
}

// ------------------------------------------------------------------
// [10] inline function : isStarCandidate
// Boundary values around rating 85 (rule: rating >= 85).
// ------------------------------------------------------------------
TEST(StarCandidateTest, RatingBoundary)
{
    player p;
    initPlayer(p, "Test Player", "T1", "Duelist", 85, 60.0f, 1.20f, "scouting");

    // just below the boundary -> not a star
    p.rating = 84;
    EXPECT_FALSE(isStarCandidate(p));

    // exactly on the boundary -> star
    p.rating = 85;
    EXPECT_TRUE(isStarCandidate(p));

    // just above the boundary -> star
    p.rating = 86;
    EXPECT_TRUE(isStarCandidate(p));
}

TEST(StarCandidateTest, KdRatioBoundary)
{
    player p;
    initPlayer(p, "Test Player", "T2", "Duelist", 90, 70.0f, 1.20f, "scouting");

    p.kdRatio = 1.19f;
    EXPECT_FALSE(isStarCandidate(p));

    p.kdRatio = 1.20f;
    EXPECT_TRUE(isStarCandidate(p));

    p.kdRatio = 1.21f;
    EXPECT_TRUE(isStarCandidate(p));
}

TEST(StarCandidateTest, WinRateBoundary)
{
    player p;
    initPlayer(p, "Test Player", "T3", "Duelist", 90, 60.0f, 1.50f, "scouting");

    p.winRate = 59.9f;
    EXPECT_FALSE(isStarCandidate(p));

    p.winRate = 60.0f;
    EXPECT_TRUE(isStarCandidate(p));
}

// ------------------------------------------------------------------
// [2.1] by value : the caller's player must not change
// ------------------------------------------------------------------
TEST(CalculateBonusTest, DoesNotChangeTheOriginalPlayer)
{
    player p;
    initPlayer(p, "Aaron Byrne", "Vortex", "Duelist", 88, 64.5f, 1.32f, "scouting");
    int ratingBefore = p.rating;

    float bonus = calculateBonus(p);

    EXPECT_GT(bonus, 0.0f);
    EXPECT_EQ(ratingBefore, p.rating);      // still the same, it was only a copy
}

// ------------------------------------------------------------------
// [5] overloaded findPlayer - by tag
// ------------------------------------------------------------------
TEST(FindPlayerTest, FindsAPlayerThatExists)
{
    player squad[4];
    int count = 0;
    fillTestSquad(squad, count);

    player *found = findPlayer(squad, count, string("Kestrel"));

    ASSERT_NE(found, nullptr);
    EXPECT_EQ("Kestrel", found->gamer_tag);
    EXPECT_EQ(91, found->rating);
}

TEST(FindPlayerTest, ReturnsNullWhenTheTagIsUnknown)
{
    player squad[4];
    int count = 0;
    fillTestSquad(squad, count);

    player *found = findPlayer(squad, count, string("Ghost"));

    EXPECT_EQ(found, nullptr);
}

// ------------------------------------------------------------------
// [7] sorting
// ------------------------------------------------------------------
TEST(SortPlayersTest, SortsHighestRatingFirst)
{
    player squad[4];
    int count = 0;
    fillTestSquad(squad, count);

    sortPlayersByRating(squad, count);

    EXPECT_EQ(91, squad[0].rating);
    EXPECT_EQ(88, squad[1].rating);
    EXPECT_EQ(79, squad[2].rating);
    EXPECT_EQ(73, squad[3].rating);
}

// ------------------------------------------------------------------
// [8] binary search on a sorted array
// ------------------------------------------------------------------
TEST(BinarySearchTest, FindsTheRatingThatExists)
{
    player squad[4];
    int count = 0;
    fillTestSquad(squad, count);
    sortPlayersByRating(squad, count);

    int index = searchByRating(squad, count, 79);

    ASSERT_NE(-1, index);
    EXPECT_EQ("Nyx", squad[index].gamer_tag);
}

TEST(BinarySearchTest, ReturnsMinusOneWhenTheRatingIsMissing)
{
    player squad[4];
    int count = 0;
    fillTestSquad(squad, count);
    sortPlayersByRating(squad, count);

    EXPECT_EQ(-1, searchByRating(squad, count, 80));
}

// ------------------------------------------------------------------
// [9] linked list shortlist
// ------------------------------------------------------------------
TEST(ShortlistTest, AddInsertsAtTheHeadAndCountsCorrectly)
{
    ShortlistNode *head = nullptr;
    player squad[4];
    int count = 0;
    fillTestSquad(squad, count);

    shortlistAdd(head, squad[0]);
    shortlistAdd(head, squad[1]);

    EXPECT_EQ(2, countShortlist(head));
    EXPECT_EQ("Nyx", head->candidate.gamer_tag);      // last added is the head

    shortlistClear(head);
}

TEST(ShortlistTest, DuplicateTagIsRejected)
{
    ShortlistNode *head = nullptr;
    player squad[4];
    int count = 0;
    fillTestSquad(squad, count);

    shortlistAdd(head, squad[0]);
    shortlistAdd(head, squad[0]);                     // same tag again

    EXPECT_EQ(1, countShortlist(head));

    shortlistClear(head);
}

TEST(ShortlistTest, RemoveDeletesTheNodeAndReturnsTrue)
{
    ShortlistNode *head = nullptr;
    player squad[4];
    int count = 0;
    fillTestSquad(squad, count);

    shortlistAdd(head, squad[0]);
    shortlistAdd(head, squad[1]);

    EXPECT_TRUE(shortlistRemove(head, "Vortex"));
    EXPECT_EQ(1, countShortlist(head));
    EXPECT_FALSE(shortlistContains(head, "Vortex"));

    // removing it a second time must report failure, not crash
    EXPECT_FALSE(shortlistRemove(head, "Vortex"));

    shortlistClear(head);
}

// ------------------------------------------------------------------
// [4] recursion
// ------------------------------------------------------------------
TEST(TotalKillsTest, SumsEveryFixture)
{
    fixture fixtures[3];
    fixtures[0].match_stats.kills = 10;
    fixtures[1].match_stats.kills = 20;
    fixtures[2].match_stats.kills = 5;

    EXPECT_EQ(35, totalKills(fixtures, 3));
    EXPECT_EQ(0, totalKills(fixtures, 0));       // base case
}

// ------------------------------------------------------------------
// [2.4] reference parameter updates the real record
// ------------------------------------------------------------------
TEST(ApplyMatchResultTest, AGreatMapLiftsTheRating)
{
    player p;
    initPlayer(p, "Liam Doyle", "Havoc", "Duelist", 70, 50.0f, 1.00f, "scouting");

    applyMatchResult(p, 20, 10);                 // K/D of 2.0 -> good map

    EXPECT_EQ(72, p.rating);
    EXPECT_GT(p.kdRatio, 1.00f);
}
