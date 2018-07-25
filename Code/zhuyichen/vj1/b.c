#include <stdio.h>

#define PI 3.1415927

int main(void)
{
    double r;

    while (scanf("%lf", &r) == 1)
        printf("%.3lf\n", 4.0 / 3 * PI * r * r * r);
}
