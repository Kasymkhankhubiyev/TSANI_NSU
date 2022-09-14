#include <stdio.h>
#include <math.h>

bool check_input_A (float A){
    if (A == 0){
        printf("коэффициент А не может быть нулевым");
        return TRUE;
    }else{ return FALSE}
}

void entrance (float* A, float* B, float* C){
    
    
    do{
    
        printf("Введите значения коэффициентов для квадратичной функции\n");
        printf("Аx^2 + Bx + C = 0, А != 0\n");
    
        scanf("%f", A);
        scanf("%f", B);
        scanf("%f", C);
    
        printf("коэффициенты из функции entrance:\n");
        printf("A = %f \nB = %f \nC = %f\n", *A, *B, *C);
    }
}

double start(){
    float A, B, C;
    entrance(&A, &B, &C);
    
    printf("коэффициенты из функции start\n");
    printf("A = %f \nB = %f \nC = %f\n", A, B, C);
}


int main()
{
    start();

    return 0;
}
