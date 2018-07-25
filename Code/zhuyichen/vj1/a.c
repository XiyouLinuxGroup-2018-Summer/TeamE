#include <stdio.h>

void swap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void char_sort(char a, char b, char c)
{
    getchar();   /*  跳过输入行末尾的\n  */
    if (a > b)
        swap(&a, &b);
    if (a > c)
        swap(&a, &c);
    if (b > c)
        swap(&b, &c);
    printf("%c %c %c\n", a, b, c);
}

int main(void)
{
    char a, b, c;

    while (scanf("%c%c%c", &a, &b, &c) == 3)
        char_sort(a, b, c);
    return 0;
}
