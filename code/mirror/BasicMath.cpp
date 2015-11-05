#include <stdlib.h>

int GetRandomSequence(int Arr[], int total)
{
	int *sequence = new int[total];

	for (int i = 0; i < total; i++)
	{
		sequence[i] = i;
	}

	int end = total - 1;
	for (int i = 0; i < total; i++)
	{
		int num = 1.0 * rand() / RAND_MAX * end;
		Arr[i] = sequence[num];
		sequence[num] = sequence[end];
		end--;
	}

	return total;
}