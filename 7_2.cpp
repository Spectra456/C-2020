#include <stdio.h>  
#include <math.h>  
#include <stdlib.h> 
#include <locale.h>
#include<time.h> 
#include <omp.h>
 

int gaussMethod(
    int m,          // Число строк матрицы
    int n,          // Число столбцов матрицы
    double *a,      // Адрес массива элементов матрицы
    double eps      
);


int main() {
    int m, n, i, j, rank;
    double *a;
    double eps, det;
    int rand_element;
    setlocale(LC_ALL, "Russian");
 
    printf("Введите размеры матрицы m, n: ");
    scanf("%d%d", &m, &n);
 
    a = (double *) malloc(m * n * sizeof(double));
    srand(time(0));
    printf("Введите элементы матрицы:\n");
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            rand_element = rand() % 100;     
            //scanf("%lf", &(a[i*n + j]));
            printf("Случайный элемент = %d\n", rand_element);

            a[i*n + j] = rand_element;
            //printf("Случайный элемент = %d\n", &a[i*n + j]);

        }
    }
 
  
    eps = 0.0001;
    rank = gaussMethod(m, n, a, 0.0001);
 
 
    printf("Ранг матрицы = %d\n", rank);
 
    if (m == n) {
        // Для квадратной матрицы вычисляем и печатаем
        //     ее определитель
        det = 1.0;
        for (i = 0; i < m; ++i) {
            det *= a[i*n + i];
        }
        //printf("Определитель матрицы = %.3lf\n", det);
    }
 
    free(a);    
    return 0;   
}
 
// Приведение вещественной матрицы
// к ступенчатому виду методом Гаусса с выбором
// максимального разрешающего элемента в столбце.
// Функция возвращает ранг матрицы
int gaussMethod(
    int m,          // Число строк матрицы
    int n,          // Число столбцов матрицы
    double *a,      // Адрес массива элементов матрицы
    double eps      // Точность вычислений
) {
    int i, j, k, l;
    double r;
 
    i = 0; j = 0;
    while (i < m && j < n) {
        // Инвариант: минор матрицы в столбцах 0..j-1
        //   уже приведен к ступенчатому виду, и строка
        //   с индексом i-1 содержит ненулевой эл-т
        //   в столбце с номером, меньшим чем j
 
        // Ищем максимальный элемент в j-м столбце,
        // начиная с i-й строки
        r = 0.0;
        #pragma omp parallel for  
        for (k = i; k < m; ++k) {
            if (fabs(a[k*n + j]) > r) {
                l = k;      // Запомним номер строки
                r = fabs(a[k*n + j]); // и макс. эл-т
            }
        }
        if (r <= eps) {
            // Все элементы j-го столбца по абсолютной
            // величине не превосходят eps.
            // Обнулим столбец, начиная с i-й строки
            #pragma omp parallel for
            for (k = i; k < m; ++k) {
                a[k*n + j] = 0.0;
            }
            ++j;      // Увеличим индекс столбца
            continue; 
        }
 
        if (l != i) {
            // Меняем местами i-ю и l-ю строки
            #pragma omp criticial  
            for (k = j; k < n; ++k) {
                r = a[i*n + k];
                a[i*n + k] = a[l*n + k];
                a[l*n + k] = (-r); // Меняем знак строки
            }
        }
 
        // Утверждение: fabs(a[i*n + k]) > eps
 
        // Обнуляем j-й столбец, начиная со строки i+1,
        // применяя элем. преобразования второго рода
        #pragma omp parallel for 

        for (k = i+1; k < m; ++k) {
            r = (-a[k*n + j] / a[i*n + j]);
 
            // К k-й строке прибавляем i-ю, умноженную на r
            a[k*n + j] = 0.0;
            for (l = j+1; l < n; ++l) {
                a[k*n + l] += r * a[i*n + l];
            }
        }
 
        ++i; ++j;   // Переходим к следующему минору
    }
 
    return i; // Возвращаем число ненулевых строк
}