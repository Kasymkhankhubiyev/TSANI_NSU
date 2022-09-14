#include <stdio.h>
#include <math.h>

#define True 1
#define False 0

void calc_discriminant(float A, float B, float C, float* Rx, float* Ry, float* Ix, float* Iy){
    float D = B*B - 4*A*C;
    printf("Discriminant = %f\n", D);
    if (D == 0){
        *Rx =  (-1 * B)/(2 * A);
        *Ry = *Rx;
    }else{
        if (D > 0){
            *Rx = (-1 * B - sqrt(D))/(2 * A);
            *Ry = (-1 * B + sqrt(D))/(2 * A);
        }else{
            *Rx = (-1 * B)/(2 * A);
            *Ry = *Rx;
            *Ix = (-1 * sqrt(-1 * D))/(2 * A);
            *Iy = -1 * (*Ix);
        }
    }
}

void peak_coord (float A, float B, float C, float* Mx, float* My){
    *Mx = (-1 * B)/(2 * A);
    *My = -1 * (B*B - 4 * A * C)/ (4 * A);
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
        scanf("%f", B);
        scanf("%f", C);
    
        printf("коэффициенты из функции entrance:\n");
        printf("A = %f \nB = %f \nC = %f\n", *A, *B, *C);
    }while (check_input_A(*A));
}

double start(){
    //инициализируем переменные
    float A, B, C;
    float Rx = 0, Ry = 0, Ix = 0, Iy = 0;
    float Mx, My;
    entrance(&A, &B, &C);
    
    calc_discriminant(A, B, C, &Rx, &Ry, &Ix, &Iy);
    printf("коэффициенты из функции start\n");
    printf("A = %f \nB = %f \nC = %f\n", A, B, C);
    
    if (Ix == 0){
        printf("Корний уравнений: \n");
        printf("x1 = %.3f\n", Rx);
        printf("x2 = %.3f\n", Ry);
    }else{
        printf("Корний уравнений: \n");
        printf("x1 = %.3f %.3fi\n", Rx, Ix);
        printf("x2 = %.3f %.3fi\n", Ry, Iy);
    }
    
    peak_coord(A, B, C, &Mx, &My);
    printf("Координаты вершин параболы :\n");
    printf("X = %.3f\n", Mx);
    printf("Y = %.3f\n", My);
    
    return 0;
}


int main()
{
    char q;
    do{
        start();
        printf("Чтобы выйти нажмите: q\n");
        printf("Чтобы повторить: y\n");
    
        q = getchar();
    }while(q != 'q');

    return 0;
}
