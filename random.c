#include <stdio.h>
#include <time.h>

#define MIN_NUMBERS 1
#define MAX_NUMBERS 100000

int main(int argc, char** argv)
{
    int count;
    if (argc != 3 || (count = strtol(argv[2],NULL,10)) < MIN_NUMBERS || count > MAX_NUMBERS)
    {
        printf("./random [file] [numbers count (from %d to %d)]\n",MIN_NUMBERS,MAX_NUMBERS);
        return 1;
    }
    FILE* file = fopen(argv[1],"w");
    srand(time(NULL));
    int i;
    for (i = 0; i < count; i++)
        fprintf(file,"%d ",rand() % 999 + 1);
    fclose(file);
    return 0;
}
