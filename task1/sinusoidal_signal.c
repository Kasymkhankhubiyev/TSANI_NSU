#include <stdio.h>
#include <math.h>
#include <time.h>
#define pi 3.14

float make_sinusoid(float A, float f, float w, float t){
    float sinus = A * sin(2 * pi * t + w);
    return sinus;
}

void start(float* A, float* f, float* w){
    printf("Введите амплитуду A:    ");
    scanf("%f", A);
    printf("Введите частоту f(Гц):      ");
    scanf("%f", f);
    printf("Введите фазу w:         ");
    scanf("%f", w);
    
    printf("%.3f,  %.3f,  %.3f\n", *A, *f, *w);
    
    
    
}

int main()
{
    float A, f, w;
    start( &A, &f, &w);

    return 0;
}
