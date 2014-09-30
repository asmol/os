#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

char error = 0;

int* read(char* path)
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

void write(char* path, int* array)
{
	FILE* file = fopen(path,"w");
	int i;
	for (i = 0; i < length(array); i++)
		fprintf(file,"%d ",array[i]);
	fclose(file);
}

void sort(int* array)
{
	int i, j, temp;
	for (i = 1; i < length(array); i++)
		for (j = i; j && array[j] < array[j-1]; j--)
		{
			temp = array[j];
			array[j] = array[j-1];
			array[j-1] = temp;
		}
}

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		puts("./sort [input file] [output file] [number of threads]");
		return 1;
	}
	int* array = read(argv[1]);
	if (!error)
	{
		sort(array);
		write(argv[2],array);
	}
	free(array);
	return 0;
}
