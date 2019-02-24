#include <iostream>
#include <cstdlib>
#include <ctime>

///Performs 2D element wise addition of the Matrices A and B and returns the result of the addition

int** rank2TensorAdd(int** A, int** B, int size)
{
    int** C = new int*[size];

    for(int i = 0; i < size; i++)
    {
        C[i] = new int[size];

        for(int j = 0; j < size; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    return C;
}


///Performs 2 dimensional matrix multiplication using the rank 2 tensor contration method

int** rank2TensorMult(int** A, int** B, int size)
{
    int**C = new int*[size];

    for(int i = 0; i < size; i++)
    {
        C[i] = new int[size];

        for(int j = 0; j < size; j++)
        {
            C[i][j] = 0;

            for(int k = 0; k < size; k++)
            {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }

    return C;
}


///Performs element wise 3D matrix addition of matrices A and B and returns the result of the addition

int*** rank3TensorAdd(int*** A, int*** B, int size)
{
    int***C = new int**[size];

    for(int i = 0; i < size; i++)
    {
        C[i] = new int*[size];

        for(int j = 0; j < size; j++)
        {
            C[i][j] = new int[size];

            for(int k = 0; k < size; k++)
            {
                C[i][j][k] = A[i][j][k] + B[i][j][k];
            }
        }
    }

    return C;
}



///Performs 3D matrix multiplication of matrices A and B using rank 3 tensor contraction method

int*** rank3TensorMult(int*** A, int*** B, int size)
{
    int***C = new int**[size];
    
    for(int i = 0; i < size; i++)
    {
        C[i] = new int*[size];

        for(int j = 0; j < size; j++)
        {
            C[i][j] = new int[size];

            for(int k = 0; k < size; k++)
            {
                C[i][j][k] = 0;

                for(int l = 0; l < size; l++)
                {
                    C[i][j][k] = C[i][j][k] + A[i][l][k]*B[l][j][k];
                }
            }
        }
    }

    return C;
}



///Utility function for generating a 2D matrix whose elements are random numbers in range [0,20]

int** rand2DMatrix(int size)
{
    int** rand_matrix = new int*[size];

    for(int i = 0; i < size; i ++)
    {
        rand_matrix[i] = new int[size];

        for(int j = 0; j < size; j++)
        {
            rand_matrix[i][j] = rand() % 21;
        }
    }

    return rand_matrix;
}



///Utility function for generating a 3D matrix whose slements are random numbers in range [0,20]

int*** rand3DMatrix(int size)
{
    int*** rand_matrix = new int**[size];
    
    for(int i = 0; i < size; i++)
    {
        rand_matrix[i] = new int*[size];

        for(int j = 0; j < size; j++)
        {
            rand_matrix[i][j] = new int[size];

            for(int k = 0; k < size; k++)
            {
                rand_matrix[i][j][k] = rand() % 21;
            }
        }
    }

    return rand_matrix;
}



int main()
{

    srand(time(NULL));

    const int N_1 = 10;
    const int N_2 = 20;

    ///Computation of the C matrix using 2d matrix addition and multiplication procedures for N = 10

    int** A_1 = rand2DMatrix(N_1);
    int** B_1 = rand2DMatrix(N_1);

    int** C_1 = rank2TensorAdd(A_1, B_1, N_1);
    int** C_2 = rank2TensorMult(A_1, B_1, N_1);


    ///Computation of the C matrix using 2d matrix addition and multiplication procedures for N = 20

    int** A_2 = rand2DMatrix(N_2);
    int** B_2 = rand2DMatrix(N_2);

    int** C_3 = rank2TensorAdd(A_2, B_2, N_2);
    int** C_4 = rank2TensorMult(A_2, B_2, N_2);


    ///Computation of the C matrix using 3d matrix addition and multiplication precedures for N = 10

    int*** A_3 = rand3DMatrix(N_1);
    int*** B_3 = rand3DMatrix(N_1);

    int*** C_5 = rank3TensorAdd(A_3, B_3, N_1);
    int*** C_6 = rank3TensorMult(A_3, B_3, N_1);
    

    ///Computation of the C matrix using 3d matrix addition and multiplication precedures for N = 20

    int*** A_4 = rand3DMatrix(N_2);
    int*** B_4 = rand3DMatrix(N_2);

    int*** C_7 = rank3TensorAdd(A_4, B_4, N_2);
    int*** C_8 = rank3TensorMult(A_4, B_4, N_2);


    return 0;
}