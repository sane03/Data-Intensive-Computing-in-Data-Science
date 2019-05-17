#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

int **allocArray(int n)
{
    int *data = malloc(n * n * sizeof(float));
    int **arr = malloc(n * sizeof(float *));

    for (int i = 0; i < n; i++)
    {
        arr[i] = &(data[i * n]);
    }

    return arr;
}

int **transposeMatrix(int **matrix, int n)
{
    int **transposed = allocArray(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            transposed[i][j] = matrix[j][i];
        }
    }

    return transposed;
}

void printArray(int **array, int rank, int n)
{
    printf("Process Number %d\n", rank);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%10d ", array[i][j]);
        }
        printf("\n");
    }
}

void getInput(int myRank, int comm_size, int *N, MPI_File fh)
{
    //bool valid = false;

    if (myRank == 0)
    {
        printf("Enter the matrix size: ");

        /*flush everything before reading user input. Prevents scanf from halting program*/
        fflush(stdout);
        scanf("%d", N);

        if (*N == 8 || *N == 16 || *N == 32 || *N == 64 || *N == 128)
        {
            MPI_File_write_at(fh, 0, N, 1, MPI_INT, MPI_STATUS_IGNORE);
        }
        else
        {
            printf("Invalid value for N");
            return;
        }
    }

    MPI_Bcast(N, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int rank, size, local_N, N, offset;
    int **inputMatrix;
    int **outputMatrix;

    MPI_Init(&argc, &argv);

    MPI_File fhw;
    MPI_File fho;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    int subArraySize = size / 8;

    //Generation of the input matrix******************************************************
    MPI_File_open(MPI_COMM_WORLD, "inputMatrixFile", MPI_MODE_CREATE | MPI_MODE_RDWR,
                  MPI_INFO_NULL, &fhw);

    //output matrix file
    MPI_File_open(MPI_COMM_WORLD, "outputMatrixFile", MPI_MODE_CREATE | MPI_MODE_RDWR,
                  MPI_INFO_NULL, &fho);

    getInput(rank, size, &N, fhw);

    //allocate memory for the input matrix to be transposed
    inputMatrix = allocArray(N);
    outputMatrix = allocArray(N);

    //Each process should generate enough elements to fill a squre submatrix of main matrix
    local_N = (N * N / size);

    offset = rank * local_N * sizeof(int) + sizeof(int);

    int local_Buffer[local_N];

    for (int i = 0; i < local_N; i++)
    {
        local_Buffer[i] = rand() % 100;
    }

    //All processes write their randomy generated matrix elements to the same file
    MPI_File_write_at_all(fhw, offset, local_Buffer, local_N, MPI_INT, &status);

    //Creating the input matrix to be transposed. This is done by the master process
    if (rank == 0)
    {
        //Start reading actual matrix data at an offset of sizeof(int) to account for the
        //added matrix size
        MPI_File_read_at(fhw, sizeof(int), &inputMatrix[0][0], N, MPI_INT, MPI_STATUS_IGNORE);

        for (int i = 1; i < size; i++)
        {
            //Fill the global matrix from elements in the inputMatrixFile
            MPI_File_read_at(fhw, i * sizeof(int) * local_N + sizeof(int), &inputMatrix[i][0], N, MPI_INT,
                             &status);
        }

        for (int row = 0; row < N; row++)
        {
            for (int col = 0; col < N; col++)
            {
                printf("%10d", inputMatrix[row][col]);
            }
            printf("\n");
        }

        ///Create the subarray MPI_Type and commit it
        MPI_Datatype subArray;
        //int displacement = N / size;
        int nDims = 2;
        int subSize[2] = {subArraySize, subArraySize};
        int mainSize[2] = {N, N};
        int rowIndex = 0;
        int colIndex = 1;

        for (int i = 1; i < size; i++)
        {
            int start[2];
            if (subArraySize * colIndex < N)
            {
                start[0] = rowIndex;
                start[1] = subArraySize * colIndex;
            }
            else
            {
                rowIndex = rowIndex + subArraySize;
                colIndex = 0;
            }

            MPI_Type_create_subarray(nDims, mainSize, subSize, start, MPI_ORDER_C,
                                     MPI_INT, &subArray);
            MPI_Type_commit(&subArray);

            MPI_Send(&(inputMatrix[0][0]), 1, subArray, i, 0, MPI_COMM_WORLD);
            MPI_Type_free(&subArray);

            colIndex++;
        }
    }
    else
    {
        int **subArray = allocArray(subArraySize);

        MPI_Recv(&(subArray[0][0]), subArraySize * subArraySize, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printArray(subArray, rank, subArraySize);

        int **transposed = transposeMatrix(subArray, subArraySize);

        printArray(transposed, rank, subArraySize);
    }
    //Creating subarrays from the data in the input matrix file

    MPI_File_close(&fhw);
    MPI_Finalize();

    return 0;
}
