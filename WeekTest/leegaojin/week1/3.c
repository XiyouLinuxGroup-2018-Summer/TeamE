/*************************************************************************
	> File Name: 3.c
	> Author:
	> Mail:
	> Created Time: 2018年07月28日 星期六 16时22分36秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
struct strange {
    char a, b, c, d, e, zero;
};

int main(int argc, char *argv[])
{
    struct strange xp[3];               //定义了一个名为xp的结构体数组
    int characters[6] = {'a','b','c','d','e'};        //定义字符串abcde
    for (int i = 0; i < 6; i++) {
        *((char *)(xp + 0) + i) = characters[i];   //将字符串中的字母赋值
                                                   //给xp[0]结构体中的6个元素
    }

    strcpy((char *)&xp[1], (char *)characters);   //给xp[1]中的元素赋值
    memcpy(&xp[2], characters, sizeof(struct strange));  //同上xp[2]
    printf("%zu\n", strlen((char *)characters));    //打印字符串的长度

    for (int i = 0; i < 3; i++) {
        printf("%c\n", xp[i].e);            //输出xp 0,1,2 中e的值
    }
}
