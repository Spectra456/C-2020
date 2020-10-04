#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {  
    double my_time,max_time,min_time,avg_time;
  
    MPI_Init(&argc, &argv);
    
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number;
    int result;
    my_time = MPI_Wtime();

    if (world_rank == 0) 
    {
        number = 0;
        int i;
        for(i = 0; i < world_size; i++)
        {   number++;
            MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
       
    } 

    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    
    printf("Process %d received number %d\n",world_rank, number);
    MPI_Barrier(MPI_COMM_WORLD);
    my_time = MPI_Wtime() - my_time; /*get time just after work section*/

    MPI_Reduce(&my_time, &max_time, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0,MPI_COMM_WORLD);
    MPI_Reduce(&my_time, &avg_time, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD); 
    if (world_rank == 0) {
    avg_time /= 4;
    printf("Min: %lf Max: %lf Avg: %lf\n", min_time, max_time,avg_time);
    }
    MPI_Finalize();
}