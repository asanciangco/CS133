#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define FIFO_SIZE 32

volatile int consumer_count, w_index, r_index, in_range, N;
volatile double * x_array, * y_array;
omp_lock_t array_lock[FIFO_SIZE];

bool buffer_full()
{
        return (w_index + 1) % FIFO_SIZE == r_index;
}

bool buffer_empty()
{
        return r_index == w_index;
}

bool put_point(double x, double y, int i)
{
        if(!omp_test_lock(&array_lock[i]))
                return false;        
        x_array[i] = x;
        y_array[i] = y;
        omp_unset_lock(&array_lock[i]);
        return true;
}

bool get_point(double *x, double *y, int i)
{
        if(!omp_test_lock(&array_lock[i]))
                return false;        
        *x = x_array[i];
        *y = y_array[i];
        omp_unset_lock(&array_lock[i]);
        return true;
}

void produce()
{
        bool ret;
        int next;
	
	#pragma omp critical
        ret = buffer_full()
		|| consumer_count + (w_index - r_index)%FIFO_SIZE > N;

        if(ret)
                return;

	#pragma omp critical
        next = (w_index + 1) % FIFO_SIZE;

        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;

        if(!put_point(x, y, next))
                return;

	#pragma omp critical
        w_index = next;
}

void consume()
{
        bool ret;
	#pragma omp critical
        ret = buffer_empty();
        
        if(ret)
                return;

        double x, y;
	#pragma omp critical
        {
                if(get_point(&x, &y, r_index))
                        r_index = (r_index + 1) % FIFO_SIZE;
        }

        x = (x - .5) * (x - .5);
        y = (y - .5) * (y - .5);
        double r = sqrt(x + y);
        int i = (r <= .5) ? 1 : 0;        

	#pragma omp critical
        {        
                in_range += i;
                consumer_count++;
        }
}


int main(int argc, char ** argv)
{
        if(argc != 2)
        {
                printf("Usage: ./hw2c N\n");
                return 1;
        }

        N = atoi(argv[1]);

        consumer_count = w_index = r_index = in_range = 0;

        x_array = malloc(sizeof(double) * FIFO_SIZE);
        y_array = malloc(sizeof(double) * FIFO_SIZE);

        int i = 0;
        for(i; i < FIFO_SIZE; i++)
                omp_init_lock(&array_lock[i]);

        while(consumer_count < N) 
	#pragma omp parallel sections
        {
		#pragma omp section
                {
                        produce();
                }
		#pragma omp section
                {
                        consume();
                }
        }
        double pi = 4.0 * in_range / consumer_count;
        printf("%.7f\n", pi);
}
