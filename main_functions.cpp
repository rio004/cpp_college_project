#include "main_functions.h"

int findBestPlayerIndex(player team[], int n)
{
  if (n == 1)
  {
    return 0;
  }
  int bestIdx = findBestPlayerIndex(team, n - 1);
  if (team[n - 1].rating > team[bestIdx].rating)
  {
    return n - 1;
  }
  return bestIdx;
}

void sortById(fixture fixtures[], int count)
{
  // bubble sort
  bool swapped;

  for (int i = 0; i < count - 1; i++)
  {
    swapped = false;
    for (int j = 0; j < count - i - 1; j++)
    {
      // move the fixture id to the next index (swap)
      if (fixtures[j].fixture_id > fixtures[j + 1].fixture_id)
      {
        fixture tmp = fixtures[j];
        fixtures[j] = fixtures[j + 1];
        fixtures[j + 1] = tmp;
        swapped = true;
      }
    }

    if (!swapped) // already sorted
      break;
  }
}

void sortById(game games[], int count)
{
  // selection sort
  for (int i = 0; i < count - 1; i++)
  {
    int min_index = i;

    // Iterate through the unsorted portion
    // to find the actual minimum
    for (int j = i + 1; j < count; j++)
    {
      if (games[j].gameid < games[min_index].gameid)
      {
        min_index = j;
      }
    }

    // Move minimum element to its
    // correct position
    if (min_index != i)
    {
      game tmp = games[i];
      games[i] = games[min_index];
      games[min_index] = tmp;
    }
  }
}