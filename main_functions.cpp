#include "main_functions.h"

int findBestPlayerIndex(player team[], int n) {
  if (n == 1) {
    return 0;
  }
  int bestIdx = findBestPlayerIndex(team, n - 1);
  if (team[n - 1].rating > team[bestIdx].rating) {
    return n - 1;
  }
  return bestIdx;
}