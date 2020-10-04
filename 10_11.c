#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int size, rank;
    double my_time,max_time,min_time,avg_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int *globaldata=NULL;
    int localdata;

    if (rank == 0) {
        globaldata = malloc(size * sizeof(int) );
        for (int i=0; i<size; i++)
            globaldata[i] = i;

        printf("Processor %d has data: ", rank);
        for (int i=0; i<size; i++)
            printf("%d ", globaldata[i]);
        printf("\n");
    }

    my_time = MPI_Wtime();

    MPI_Scatter(globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    my_time = MPI_Wtime() - my_time; /*get time just after work section*/

    MPI_Reduce(&my_time, &max_time, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0,MPI_COMM_WORLD);
    MPI_Reduce(&my_time, &avg_time, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);
    
    
    printf("Process %d received number %d\n", rank, localdata);

    // MPI_Gather(&localdata, 1, MPI_INT, globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // if (rank == 0) {
    //     printf("Processor %d has data: ", rank);
    //     for (int i=0; i<size; i++)
    //         printf("%d ", globaldata[i]);
    //     printf("\n");
    // }

    if (rank == 0)
        free(globaldata);

    if (rank == 0) {
    avg_time /= 4;
    printf("Min: %lf Max: %lf Avg: %lf\n", min_time, max_time,avg_time);
    }
    MPI_Finalize();
    
    return 0;
}