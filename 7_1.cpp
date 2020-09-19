#include <iostream>
#include <math.h>
#include <omp.h>
#define N 10
int main(int argc, char** argv) {
    double x[N],y[N];
    double res[N];
    x[0] = 2;
    y[0] = 2;
    omp_set_num_threads(2);
    //#pragma omp parallel
    //{ 
    #pragma omp parallel for
    for (int i=1; i<N; i++) {      
    x[i] = x[i-1]*x[i-1];
    y[i] = x[i] /y[i-1];
    int tid = omp_get_thread_num();
    std::cout << "Thread №" << tid << " " <<x[i] << std::endl;
    res[i] = x[i];
     }
    //}

    y[0]=x[N-1]; 
    
    return 0;
}