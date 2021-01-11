#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    long n = 100000000, i, count = 0;
    double z, pi, x, y;
    int numt = 2;

    omp_set_num_threads(numt);

    double starttime = omp_get_wtime();
#pragma omp parallel
    {
        srand(omp_get_wtime() * omp_get_thread_num());
        printf("Thread: %d\n", omp_get_thread_num());
#pragma omp for private(x, y, z) reduction(+ \
                                           : count)
        for (i = 0; i < n; i++)
        {
            x = (double)rand() / RAND_MAX;
            y = (double)rand() / RAND_MAX;
            z = x * x + y * y;
            if (z <= 1)
                count++;
        }
    }
    pi = (double)count / n * 4;
    double finishtime = omp_get_wtime();

    printf("# of trials= %d , estimate of pi is %g \n", n, pi);
    printf("\nExecution time is : %f", (double)(finishtime - starttime));

    return 0;
}