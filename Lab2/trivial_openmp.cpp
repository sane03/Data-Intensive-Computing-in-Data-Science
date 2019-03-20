#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include <time.h>
#include <sys/time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

/*  Generates and Populates an n by n square matrix A with random numbers*/
int** randMatrix(int n)
{
    int** A = new int*[n];

    for(auto i = 0; i < n; i++)
    {
        A[i] = new int[n];

        for(auto j = 0; j < n; j++)
        {
            A[i][j] = rand() % n;
        }
    }

    return A;
}


/* Transposes the matrix A using a trivial in place algorithm. It then uses openmp
to parallelize the function*/

int** trivialInplaceTranspose(int**A, int n)
{

#pragma omp parallel for
    for(auto i = 0; i < n -1; i++)
    {
#pragma omp parallel for
        for(auto j = i + 1; j < n; j++)
        {
            std::swap(A[i][j], A[j][i]);
        }
    }
    return A;
}


/* Determines the time taken to perform the tranpose operation on matrix A */
void benchMark(int** A, int n)
{
    struct timeval start, end;

    gettimeofday(&start, NULL);

    trivialInplaceTranspose(A, n);    

    gettimeofday(&end, NULL);

    double elapsed = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/ 1e6;

    std::cout << "Trivial transposition for N = " << n << ": " << elapsed << std::endl;
}


int main()
{
    srand(time(NULL));

    std::vector<int> sizes{128, 1024, 2048, 4096, 8192, 16384};

    int** matrix;

    for(auto n : sizes)
    {
        matrix = randMatrix(n);
        benchMark(matrix,n);

        for(auto i = 0; i <n; i++)
        {
            delete[] matrix[i];
        }

        delete[] matrix;
    }

    return 0;
}