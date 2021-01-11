#include<stdio.h>
#include<mpi.h>

int factorial(int a) {

    int ret = 1;

    while(a) {
        ret *= a--;
    }

    return ret;
}

void process0() {

    double wtime = MPI_Wtime();

    int input1 = 10;
    int input2 = 5;

    MPI_Send( &input1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD );
    MPI_Send( &input2, 1, MPI_INT, 2, 2, MPI_COMM_WORLD );
    printf("Sent inputs from PROCESSOR 0\n");

    int output1;
    int output2;
    MPI_Status status;

    MPI_Recv( &output1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status );
    MPI_Recv( &output2, 1, MPI_INT, 2, 2, MPI_COMM_WORLD, &status );

    wtime = MPI_Wtime() - wtime;

    printf("Process 1 output: %d\n", output1);
    printf("Process 2 output: %d\n", output2);
    printf("Process 0 execution time: %f\n", wtime);

}

void process1() {
    int input;
    MPI_Status status;

    MPI_Recv( &input, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status );
    printf("Process 1 received input\n");

    double wtime = MPI_Wtime();    
    int output = input * input;
    wtime = MPI_Wtime() - wtime;

    MPI_Send(&output, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    printf("Process 1 execution time: %f\n", wtime);
}

void process2() {
    int input;
    MPI_Status status;

    MPI_Recv( &input, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status );
    printf("Process 2 received input\n");

    double wtime = MPI_Wtime();    
    int output = factorial(input);
    wtime = MPI_Wtime() - wtime;

    MPI_Send(&output, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    printf("Process 2 execution time: %f\n", wtime);
}

int main( int argc, char *argv[] ){

    int ierr;
    int pid;

    ierr = MPI_Init(&argc, &argv);

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    switch(pid){
        case 0: process0(); break;
        case 1: process1(); break;
        case 2: process2(); break;
    }

    ierr = MPI_Finalize();

}