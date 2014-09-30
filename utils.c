#include <stdlib.h>
#include "utils.h"

int length(int array[])
{
	int count = 0;
	while (array[++count]);
	return count;
}
