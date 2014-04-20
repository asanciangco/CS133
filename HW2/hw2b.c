#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int is_prime(int p)
{
	int i;
	if (p < 2)
		return 0;
	i = 2;
	while (i * i <= p)
	{
		if (p % i == 0)
			return 0;
 		i++;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Invalid arguments!\n");
		return 0;
	}
	int n = atoi(argv[1]);
	int i;
	int *primes = (int*)malloc(sizeof(int) * (n + 1));

	omp_set_num_threads(atoi(argv[2]));

	#pragma omp parallel for schedule(dynamic, 32)
	for (i = 3; i <= n; i += 2)
		if (is_prime(i))
			primes[i] = 1;
	
	printf("2");
	for (i = 3; i <= n; i++)
		if (primes[i])
			printf(", %i", i);
	printf("\n");

	free(primes);

	return 0;
}
