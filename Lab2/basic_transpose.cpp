
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <vector>

#include <time.h>
#include <sys/time.h>


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


/* Transposes the matrix A using the basic approach (not in place)  */
int** basicTranspose(int** A, int n)
{
    int** T = new int*[n];

    for(auto i = 0; i < n; i++)
    {
        T[i] = new int[n];

        for(auto j = 0; j < n; j++)
        {
            T[i][j] = A[j][i];
        }
    }
    return T;
}


/* Determines the time taken to perform the tranpose operation on matrix A */
void benchMark(int** A, int n)
{
    struct timeval start, end;

    gettimeofday(&start, NULL);

    basicTranspose(A, n);    

    gettimeofday(&end, NULL);

    double elapsed = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/ 1e6;

    std::cout << "Basic transposition for N = " << n << ": " << elapsed << std::endl;
}


int main()
{
    srand(time(NULL));

    std::vector<int> sizes{128, 1024, 2048, 4096, 8192};

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
