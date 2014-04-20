#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <sys/time.h>

#include "cdf97.h"
#include "util.h"

int main(int argc, char *argv[]) {

    struct	timeval t1, t2, tr;
    double	elapsedTime = 0;

	if(argc<2) {
		printf("./dwt2d input.bmp [level]");
		return -1;
	}

	char* fname = argv[1];
	char fname_out[50] = "dwt_out.bmp";

	int level = 1;

	if (argc>2)
		level = atoi(argv[2]);

	int N1;
	int	N2;
	int i, j;

	float *img;
	float *tmp;
	
	// read image from a BMP file
	int err = imread(&img, &N1, &N2, fname);
	if (err != 0) return err;

	tmp = (float*)calloc(N1*N2, sizeof(float));
    gettimeofday(&t1, NULL);

	// start 2D DWT transform
	cdf97(img, tmp, N1, N2, level);

    gettimeofday(&t2, NULL);
    timersub(&t1, &t2, &tr);
    printf("Execute time: %.2f sec\n", fabs(tr.tv_sec+(double)tr.tv_usec/1000000.0));

	// write an BMP image
	imwrite(img, N1, N2, fname_out);

	free(img);
	free(tmp);

	return 0;
}
