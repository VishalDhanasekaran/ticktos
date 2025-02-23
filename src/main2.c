#include <stdio.h>
#include <stdlib.h>
#include "pcb.c"
#include "queue.c"
#include "../include/lock.h"
#include <time.h>

void bubbleSortTask() 
{
    int arr[] = {5, 3, 8, 1, 2};
    int n = 5;
    printf("\nTask %s running: Sorting array", currentTask->pname);
    
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = 0; j < n - i - 1; j++) 
        {
            if (arr[j] > arr[j + 1]) 
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printf("\nTask %s completed. Sorted Array: ", currentTask->pname);
    for (int i = 0; i < n; i++) 
    {
        printf("%d ", arr[i]);
    }
}

void fibonacciTask() 
{
    int n = 10;
    int t1 = 0, t2 = 1;
    printf("\nTask %s running: Fibonacci sequence", currentTask->pname);
    
    printf("\nFibonacci Sequence: %d %d ", t1, t2);
    for (int i = 3; i <= n; i++) 
    {
        int nextTerm = t1 + t2;
        printf("%d ", nextTerm);
        t1 = t2;
        t2 = nextTerm;
    }
    printf("\nTask %s completed.", currentTask->pname);
}

void matrixMultiplicationTask() 
{
    int A[2][2] = {{1, 2}, {3, 4}};
    int B[2][2] = {{5, 6}, {7, 8}};
    int C[2][2] = {0};
    
    printf("\nTask %s running: Matrix Multiplication", currentTask->pname);
    
    for (int i = 0; i < 2; i++) 
    {
        for (int j = 0; j < 2; j++) 
        {
            C[i][j] = 0;
            for (int k = 0; k < 2; k++) 
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    printf("\nTask %s completed. Result Matrix: ", currentTask->pname);
    for (int i = 0; i < 2; i++) 
    {
        printf("\n");
        for (int j = 0; j < 2; j++) 
        {
            printf("%d ", C[i][j]);
        }
    }
}

int main() 
{
    srand(time(NULL));
    initializeQueue(&readyQ);

    createTask("SortingTask", 10, bubbleSortTask);
    createTask("FibonacciTask", 20, fibonacciTask);
    createTask("MatrixMultTask", 30, matrixMultiplicationTask);

    OS_Run();
    return 0;
}

