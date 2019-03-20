

#include <omp.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include <time.h>
#include <sys/time.h>


/* Number of threads used for parallelising the algorithm. 4 was found to
   be more efficient for the laptop used for testing */
#define NUM_THREADS 4

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

/* Swaps the elements on the right of matrix diagonal element at index(i,i)
   by the elements below this matrix diagonal element. The matrix is a 
   square matrix of dimensions n by n*/
void diagonalRowColumnExchange(int** A, int n, int index)
{
    auto start = omp_get_thread_num() + index + 1;
    auto increament = omp_get_num_threads();

    for(auto i = start; i < n; i += increament)
    {
        std::swap(A[index][i], A[i][index]);
    }
}


/* Uses the diagonal algorithm to find the tranpose of a square matrix in place.
   For each diagonal element, threads are created which interchange corresponding
   row and column elements below(column) and on the right(row) of the diagonal
   element */
int** threadedDiagonalMatrixTranspose(int**A, int n)
{
    for(auto i = 0; i < n; i++)
    {
        #pragma omp parallel num_threads(NUM_THREADS)
        diagonalRowColumnExchange(A, n, i);
    }

    return A;
}


/* Determines the time taken to perform the tranpose operation on matrix A */
void benchMark(int** A, int n)
{


    struct timeval start, end;

    gettimeofday(&start, NULL);

    threadedDiagonalMatrixTranspose(A, n);    

    gettimeofday(&end, NULL);

    double elapsed = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/ 1e6;

    std::cout << "Threaded diagonal transposition for N = " << n << ": " << elapsed << std::endl;
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
