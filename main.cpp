/*
    Description:
    Esports team could scout for professional player for their team
*/

#include <iostream>

#include "main_functions.h"

using namespace std;

int main(void)
{
    // Arrays holding everything the program reads from the CSV files
    player players[MAX_PLAYERS];
    int numPlayers = 0;

    fixture fixtures[MAX_FIXTURES];
    int numFixtures = 0;

    game games[MAX_FIXTURES];
    int numGames = 0;

    // The two data files live next to the source files
    readingInputfiles("existingPlayers.csv", players, &numPlayers);
    readingFixturesInputFiles("fixtures.csv", fixtures, &numFixtures);

    // There is no separate games file, so the unique games are taken
    // from the fixtures that were just read
    for (int i = 0; i < numFixtures && numGames < MAX_FIXTURES; i++)
    {
        bool alreadyAdded = false;
        for (int j = 0; j < numGames; j++)
        {
            if (games[j].gameid == fixtures[i].match_stats.gameid)
            {
                alreadyAdded = true;
                break;
            }
        }
        if (!alreadyAdded)
        {
            games[numGames] = fixtures[i].match_stats;
            numGames++;
        }
    }

    cout << "Loaded " << numPlayers << " players and " << numFixtures
         << " fixtures from the CSV files." << endl;

    // Hand everything over to the menu loop
    runMenu(players, numPlayers, fixtures, numFixtures, games, numGames);

    // Save the source files
    writeToOutputFile("existingPlayers.csv", players, numPlayers);
    writingToFixturesOutputFile("fixtures.csv", fixtures, numFixtures);

    return 0;
}