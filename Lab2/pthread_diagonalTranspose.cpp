#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <sys/time.h>

#define NUM_THREADS 4

int** matrix;

struct ThreadInfo
{
    int thread_id;
    int matrix_size;
    int diagonal_index;
};


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


void* diagonalRowColumnExchange(void* threadArg)
{
    struct ThreadInfo* data;
    data = (struct ThreadInfo*) threadArg;

    auto start = data -> thread_id + data -> diagonal_index + 1;
    auto increament = NUM_THREADS;
    auto index = data -> diagonal_index;

    for(auto i = start; i < data -> matrix_size; i+= increament)
    {
        std::swap(matrix[index][i], matrix[i][index]);
    }

    pthread_exit(NULL);
}


int** threadedDiagonalMatrixTranspose(int n)
{
    pthread_t threads[NUM_THREADS];

    struct ThreadInfo thread_data[NUM_THREADS];

    for(auto i = 0; i < n; i++)
    {
        for(auto j = 0; j < NUM_THREADS; j++)
        {
            thread_data[j].thread_id = j;
            thread_data[j].diagonal_index = i;
            thread_data[j].matrix_size = n;

            /* Create NUM_THREADS threads and let each thread perform the row col exchange */
            pthread_create(&threads[j], NULL, diagonalRowColumnExchange, (void*)&thread_data[j]);
        }

        for(auto k = 0; k < NUM_THREADS; k++)
        {
            pthread_join(threads[k], NULL);
        }
    }

    return matrix;
}


/* Determines the time taken to perform the tranpose operation on matrix A */
void benchMark(int n)
{

    struct timeval start, end;

    gettimeofday(&start, NULL);

    threadedDiagonalMatrixTranspose(n);    

    gettimeofday(&end, NULL);

    double elapsed = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/ 1e6;

    std::cout << "Threaded diagonal transposition for N = " << n << ": " << elapsed << std::endl;
}

int main()
{

    srand(time(NULL));

    std::vector<int> sizes{128, 1024, 2048, 4096, 8192, 16384};

    for(auto n : sizes)
    {
        matrix = randMatrix(n);
        benchMark(n);

        for(auto i = 0; i <n; i++)
        {
            delete[] matrix[i];
        }

        delete[] matrix;
    }

    return 0;
}