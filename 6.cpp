#include<omp.h>
#include <stdlib.h>
#include <iostream>
#include <chrono> 
   
bool isPrime(int n) 
{ 
    // Corner case 
    if (n <= 1) 
        return false; 
  
    // Check from 2 to n-1 
    for (int i = 2; i < n; i++) 
        if (n % i == 0) 
            return false; 
  
    return true; 
} 

int main(int argc, char** argv) {

    const unsigned long array_size =  99999;
    int arr[array_size];
    bool res_omp[array_size];
    bool res_single[array_size];
    int i; 

    for (int i = 0; i < array_size; i++)
    {
        arr[i] = i + 1;
    }
    std::cout <<"max threads: "<< omp_get_max_threads() << std::endl;

    if (omp_get_max_threads() < 4) {
         omp_set_num_threads(omp_get_max_threads());
        } else {
           omp_set_num_threads(8);
        }

    auto start_omp = std::chrono::high_resolution_clock::now(); 
    #pragma omp parallel  
    for(i=0; i < array_size; i++) {
    res_omp[i] = isPrime(arr[i]);
    //std::cout << arr[i] << " - is prime number: "  << std::boolalpha << res_omp[i] << std::endl;
    }
    auto stop_omp = std::chrono::high_resolution_clock::now(); 
    auto duration_omp = std::chrono::duration_cast<std::chrono::milliseconds>(stop_omp - start_omp); 

    std::cout <<"OMP handling duration: "<< duration_omp.count() << " milliseconds" << std::endl; 
    
    auto start_sng = std::chrono::high_resolution_clock::now();
    for(i=0; i < array_size; i++) {
    res_single[i] = isPrime(arr[i]);
    //std::cout << arr[i] << " - is prime number: "  << std::boolalpha << res_single[i] << std::endl;
    }
    auto stop_sng = std::chrono::high_resolution_clock::now();
    auto duration_sng = std::chrono::duration_cast<std::chrono::milliseconds>(stop_sng - start_sng); 
    std::cout <<"Single thread handling duration: "<< duration_sng.count() << " milliseconds" << std::endl; 

    return(0);
    }