#include <stdio.h>
#include <stdlib.h>
#include "defs.h"


        
int main()
{
    all_init();

    int move = 0;
    int from = A2; int to = H7;
    int cap = wR; int promote = bQ;

    move = (( from ) | (to << 7) | (cap<<14) | (promote <<20));

    printf("from: %d to: %d, cap: %d, promotion: %d\n", FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move));
      printf("Algebraic from: %s\n", print_square(from));
    printf("Algebraic to: %s\n", print_square(to));
    printf("algebraic move: %s\n", print_move(move));

    return 0;
}
