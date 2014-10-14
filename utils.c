#include "utils.h"

int length(const int* const array)
{
    int count = 0;
    while (array[++count]);
    return count;
}
