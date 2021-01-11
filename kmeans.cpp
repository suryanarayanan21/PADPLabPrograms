#include <iostream>
#include <vector>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

using namespace std;

struct point
{
    float x;
    float y;
    int label;
};

float distance(point a, point b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

vector<point> kmeansIteration(vector<point> points, vector<point> centers)
{
#pragma omp parallel for
    for (int i = 0; i < points.size(); ++i)
    {
        float min = distance(points[i], centers[0]);
        points[i].label = 0;

        for (int j = 1; j < centers.size(); ++j)
        {

            float temp = distance(points[i], centers[j]);

            if (temp < min)
            {
                temp = min;
                points[i].label = j;
            }
        }
    }

    return points;
}

vector<point> createSamplePoints(int size)
{
    vector<point> points(size);

    for (int i = 0; i < size; ++i)
    {
        points[i].x = rand() % 100;
        points[i].y = rand() % 100;
        points[i].label = 0;
    }

    return points;
}

int main()
{
    int numpoints = 100000;
    int numclusters = 4;
    int numthreads = 8;

    srand(10);

    vector<point> centers = {{25, 75, 0}, {75, 75, 0}, {25, 25, 0}, {75, 25, 0}};
    vector<point> points = createSamplePoints(numpoints);

    omp_set_num_threads(numthreads);

    double start = omp_get_wtime();
    points = kmeansIteration(points, centers);
    double finish = omp_get_wtime();

    cout << "Execution time = " << finish - start;

    return 0;
}