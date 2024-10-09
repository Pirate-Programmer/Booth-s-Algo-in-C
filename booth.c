#include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

int* toBinary(int,int);
int ars(int*,int, int);
void add(int* ,int* ,int);
long result(int * A, int * Q, int size);

#define MAX(a,b) (a > b)? a: b;

int main(int argc, char** argv) {
    
    if(argc != 2)
        return 1;
    if(strspn(argv[1],"0123456789*- ") != strlen(argv[1]))
        return 2;
        printf("Expression: %s\n",argv[1]);
        char * str = (char* )malloc(sizeof(char) * 9);
        sscanf(argv[1],"%s",str);
        int decimal_Q = atoi(str);

        sscanf(argv[1],"%*s %*s %s",str);
        int decimal_M = atoi(str);

        //printf("%d %d\n",decimal_Q,decimal_M);
        free(str);
    
    int Q_size = (int)log2(abs(decimal_Q)) + 1;
    int M_size = (int)log2(abs(decimal_M)) + 1;

    int size = 1;
    size += MAX(Q_size,M_size);

    //Intialization
    int* Q = toBinary(decimal_Q,size);
    int* ADD_M = toBinary(decimal_M,size);
    int* SUB_M = toBinary(-1*decimal_M,size);
    int* A = (int* )calloc(sizeof(int),size);
    int Q_minus = 0;
    int count = size;
    
    while(count > 0)
    {
        //A = A + SUB_M
        if(Q[size - 1] == 1 && Q_minus == 0)
            add(A,SUB_M,size);
        //A = A + ADD_M
        else if(Q[size - 1] == 0 && Q_minus == 1)
            add(A,ADD_M,size);

        //ARS
        int bit = ars(A,size,A[0]);
        Q_minus = ars(Q,size,bit);
        count--;
    }

    // for(int i = 0; i < size; i++)
    //     printf("%d ",A[i]);
    // //printf("");

    //     for(int i = 0; i < size; i++)
    //     printf("%d ",Q[i]);
    // printf("\n");

   printf("Result: %ld\n",result(A,Q,size));
    free(Q);
    free(ADD_M);
    free(SUB_M);
    free(A);

    return 0;
}


int* toBinary(int num,int size)
{
    int no_of_bits = (int)log2(num) + 1;
    int* bin = (int* )calloc(sizeof(int),size);
    if(bin == NULL)
    {
        printf("Insufficient Memory\n");
        exit(EXIT_FAILURE);
    }
    for(int i = size - 1,idx = 0; i >= 0; i--, idx++)
        bin[idx] = ((num >> i) & 1);

    return bin;
}

int ars(int* num,int numSize, int bit)
{

    int temp = num[numSize - 1];
    for(int i = numSize - 1; i > 0; i--)
        num[i] = num[i-1];
    num[0] = bit;
    return temp;
}

void add(int* A,int* M,int size)
{
    for(int i = size - 1,sum = 0,carry = 0; i >= 0; i--)
    {  
        int sum = A[i] + M[i];
        A[i] = (sum + carry)%2;
        carry = (sum + carry) / 2;
    }
}

long result(int * A, int * Q, int size)
{
    if(A == NULL || Q == NULL)
        return -1;

    long result = 0,flag = 0;
    if(A[0] == 1)  //take 2's complement
    {
        flag = 1;
        int rem = 1;
        for(int i = size - 1; i >= 0; i--)
        {
            int temp = Q[i]^1;
            Q[i] = (temp + rem)%2;
            rem = (temp + rem)/2;
        }
        for(int i = size - 1; i >= 0; i--)
        {
            int temp = A[i]^1;
            A[i] = (temp + rem)%2;
            rem = (temp + rem)/2;         
        }
    }

    for(int i = size - 1,idx = 0; i >= 0; i--,idx++)
    {
        if(Q[i] == 1)
            result+=(int)pow(2,idx);
        if(A[i] == 1)
            result+=(int)pow(2,idx + size);
    }

    return (flag == 1)?result * -1: result;

}