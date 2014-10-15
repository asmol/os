#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "utils.h"

#define MIN_THREADS 1
#define MAX_THREADS 8

struct sortargs
{
    int* array;
    int start, end;
};

char error = 0;

int* read(const char* path)
{
    int* result = (int*)malloc(sizeof(int));
    FILE* file = fopen(path,"r");
    int count = 0;
    if (!file)
    {
        perror(path);
        error = 1;
    }
    else
    {
        while (!feof(file))
        {
            if (!fscanf(file,"%d",&result[count++]))
            {
                puts("Error reading file");
                error = 1;
                break;
            }
            result = (int*)realloc(result,(count + 1) * sizeof(int));
        }
        fclose(file);
    }
    result[count] = 0;
    return result;
}

void write(const char* path, const int* const array)
{
    FILE* file = fopen(path,"w");
    if (!file)
    {
        perror(path);
        error = 1;
        return;
    }
    int i;
    for (i = 0; i < length(array); i++)
        fprintf(file,"%d ",array[i]);
    fclose(file);
}

void* sort(void* const args)
{
    struct sortargs* sargs = args;
    int i, j, temp;
    for (i = sargs -> start; i < sargs -> end; i++)
        for (j = i; j > sargs -> start && sargs -> array[j] < sargs -> array[j-1]; j--)
        {
            temp = sargs -> array[j];
            sargs -> array[j] = sargs -> array[j-1];
            sargs -> array[j-1] = temp;
        }
    free(sargs);
    return NULL;
}

struct sortargs* mksargs(int* const array, int start, int end)
{
    struct sortargs* sargs = (struct sortargs*)malloc(sizeof(struct sortargs));
    sargs -> array = array;
    sargs -> start = start;
    sargs -> end = end;
    return sargs;
}

int main(int argc, char** argv)
{
    int count;
    if (argc != 4 || (count = strtol(argv[3],NULL,10)) < MIN_THREADS || count > MAX_THREADS)
    {
        printf("./sort [input file] [output file] [threads count (from %d to %d)]\n",MIN_THREADS,MAX_THREADS);
        return 1;
    }
    int* array = read(argv[1]);
    clock_t start, end;
    if (!error)
    {
        int len = length(array);
        if (count > 1)
        {
            count = count <= len / 2 ? count : len / 2;
            int step, i;
            step = len / count;
            pthread_t threads[count];
            start = clock();
            for (i = 0; i < count; i++)
                if (pthread_create(&threads[i],NULL,sort,(void*)mksargs(array,i * step,i < count ? (i + 1) * step : len)))
                {
                    puts("Error creating thread");
                    return 1;
                }
            for (i = 0; i < count; i++)
                if (pthread_join(threads[i],NULL))
                {
                    puts("Error joining thread");
                    return 1;
                }
        }
        if (!start)
            start = clock();
        sort((void*)mksargs(array,0,len));
        end = clock();
        printf("Time passed: %.2f s\n",(double)(end - start) / CLOCKS_PER_SEC);
        write(argv[2],array);
    }
    free(array);
    return 0;
}
