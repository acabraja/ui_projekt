#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 7
#define HEIGHT 6
#define ORANGE_WINS  1000000
#define YELLOW_WINS -1000000
#define ORANGE 1
#define YELLOW -1
#define BARREN 0

int minMax(int board[][WIDTH]);
