#include "grader.h"
#include "memory.h"

int card[60];

void play() {
  for (int i = 1; i <= 50; ++i) card[i] = faceup(i);
  for (int i = 1; i <= 50; ++i)
    for (int j = 1; j < i; ++j)
      if (card[i] == card[j]) {
        faceup(i);
        faceup(j);
      }
}
