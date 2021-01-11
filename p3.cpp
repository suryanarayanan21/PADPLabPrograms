#include <iostream>
#include <vector>
#include <math.h>
#include <omp.h>

using namespace std;

int count_ones(vector<bool> primes)
{
    long count = 0;
#pragma omp parallel for reduction(+ \
                                   : count)
    for (long i = 2; i < primes.size(); ++i)
    {
        if (primes[i])
            count++;
    }
    return count;
}

int display(vector<bool> primes)
{
    for (long i = 2; i < primes.size(); ++i)
    {
        if (primes[i])
            cout << i << " ";
    }

    return 0;
}

void strike(vector<bool> &primes, long start, int stride, long limit)
{
    for (long i = start; i <= limit; i += stride)
    {
        primes[i] = false;
    }
}

vector<bool> simpleSieve(long n)
{
    long m = sqrt(n);
    vector<bool> primes(n + 1, true);

    for (int i = 2; i <= m; ++i)
    {
        if (primes[i])
            strike(primes, 2 * i, i, n + 1);
    }

    return primes;
}

vector<bool> cacheFriendlySieve(long n)
{
    // Perform simple sieve on segments of array
    long m = sqrt(n);
    vector<bool> primes(n + 1, true);

    vector<bool> first_primes = simpleSieve(m);

    for (long i = 1; i <= m; ++i)
    {
        primes[i] = first_primes[i];
    }

    for (long i = m + 1; i < n; i += m)
    {
        long first = i;
        long limit = min(i + m, n);

        for (long j = 2; j <= m; ++j)
        {
            if (first_primes[j])
            {
                long start = floor(first / j) * j;
                if (start < first)
                    start += j;
                strike(primes, start, j, limit);
            }
        }
    }

    return primes;
}

vector<bool> cacheFriendlySieveParallel(long n)
{
    // Perform simple sieve on segments of array
    long m = sqrt(n);
    vector<bool> primes(n + 1, true);

    vector<bool> first_primes = simpleSieve(m);

    for (long i = 1; i <= m; ++i)
    {
        primes[i] = first_primes[i];
    }

#pragma omp parallel for shared(primes)
    for (long i = m + 1; i < n; i += m)
    {
        long first = i;
        long limit = min(i + m, n);

        for (long j = 2; j <= m; ++j)
        {
            if (first_primes[j])
            {
                long start = floor(first / j) * j;
                if (start < first)
                    start += j;
                strike(primes, start, j, limit);
            }
        }
    }

    return primes;
}

int main()
{
    omp_set_num_threads(8);
    double time0 = omp_get_wtime();
    vector<bool> primesUnfriendly = simpleSieve(100000000);
    double time1 = omp_get_wtime();
    vector<bool> primesFriendly = cacheFriendlySieve(100000000);
    double time2 = omp_get_wtime();
    vector<bool> primesFriendlyParallel = cacheFriendlySieve(100000000);
    double time3 = omp_get_wtime();

    cout << "Count Unfriendly = " << count_ones(primesUnfriendly) << '\n';
    cout << "Execution time = " << (double)(time1 - time0) << '\n';
    cout << "Count Friendly = " << count_ones(primesFriendly) << '\n';
    cout << "Execution time = " << (double)(time2 - time1) << '\n';
    cout << "Count Friendly Parallel = " << count_ones(primesFriendlyParallel) << '\n';
    cout << "Execution time = " << (double)(time3 - time2) << '\n';

    return 0;
}