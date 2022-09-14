
#include <stdio.h>
#include <math.h>

#define True 1
#define False 0

void calc_discriminant(float A, float B, float C, float* Rx, float* Ry, float* Ix, float* Iy){
    float D = B**2 - 4*A*C;
    if (D = 0){
        *Rx =  (-1 * B)/(2 * a);
        *Ry = *Rx;
    }else{
        if (D > 0){
            *Rx = (-1 * B - sqrt(D))/(2 * A);
            *Ry = (-1 * B + sqrt(D))/(2 * A);
        }else{
            *Rx = (-1 * B)/(2 * a);
            *Ry = *Rx;
            *Ix = (-1 * sqrt(-1 * D))/(2 * a);
            *Iy = -1 * (*Ix);
        }
    }
}

int check_input_A (float A){
    //проверяем значение коэффициента А //
    if (A == 0){
        printf("коэффициент A не может быть нулевым \n");
        return True;
    }else{ return False;}
}

void entrance (float* A, float* B, float* C){
    
    
    do{ //проверяем на наличие ненулевого коэффициента А //
        printf("Введите значения коэффициентов для квадратичной функции\n");
        printf("Аx^2 + Bx + C = 0, А != 0\n");
    
        scanf("%f", A);
        float D = -1 * A;
        printf("%d\n", D);
        scanf("%f", B);
        scanf("%f", C);
    
        printf("коэффициенты из функции entrance:\n");
        printf("A = %f \nB = %f \nC = %f\n", *A, *B, *C);
    }while (check_input_A(*A));
}

double start(){
    //инициализируем переменные
    float A, B, C;
    floar Rx = 0, Ry = 0, Ix = 0, Iy = 0;
    entrance(&A, &B, &C);
    
    printf("коэффициенты из функции start\n");
    printf("A = %f \nB = %f \nC = %f\n", A, B, C);
}


int main()
{
    start();

    return 0;
}
