#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

void main(int argc, char*argv[]){

	if (argc < 3)
	{
		printf("./check file1.dat file2.dat\n");
		exit(1);
	}

	char * fname1 = argv[1];
	char * fname2 = argv[2];

	FILE* f1 = fopen(fname1, "rb");
	FILE* f2 = fopen(fname2, "rb");
	assert(f1 != NULL && f2 != NULL);

	int m1, m2, n1, n2, p1, p2;
	
	fread(&m1, 1, sizeof(int), f1);
	fread(&n1, 1, sizeof(int), f1);
	fread(&p1, 1, sizeof(int), f1);

	fread(&m2, 1, sizeof(int), f2);
	fread(&n2, 1, sizeof(int), f2);
	fread(&p2, 1, sizeof(int), f2);

	assert(m1 == m2 && n1 == n2 && p1 == p2);

	float *img1 = (float*)calloc(n1*m1*p1, sizeof(float));
	float *img2 = (float*)calloc(n1*m1*p1, sizeof(float));
	assert(img1 != NULL && img2 != NULL);

	fread (img1, n1*m1*p1, sizeof(float), f1);
	fread (img2, n1*m1*p1, sizeof(float), f2);

	int i, j, k;
	double diff = 0;
	for (i = 0; i < m1; i++)
		for (j = 0; j < n1; j++)
			for(k = 0; k < p1; k++)
				diff += fabs(img1[i * n1 * p1 + j * p1 + k] - img2[i * n1 * p1 + j * p1 + k]);

	printf("diff = %lf\n", diff);

	fclose(f1);
	fclose(f2);
	free(img1);
	free(img2);
}
