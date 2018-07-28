#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
/*
typedef struct tag_unimportant {
    char *name;
    int num;
} A;
void func(A **a)
{
    *a = (A *)malloc(sizeof(A));
    //*name有问题
	(*a)->name = (char *)calloc(sizeof(100),1); 
    strcpy((*a)->name, "Xiyou Linux Group");
    (*a)->num = 20180728;
}
int main(int argc, char *argv[])
{
    A *a;
    func(&a);	//传参有问题，一级传一级，改不了这边的 
    printf("%s %d\n", a->name, a->num);
    return 0;
}
*/
struct strange {			//定义一个结构体 
    char a, b, c, d, e, zero;		//结构体里边定义变量 
};

int main(int argc, char *argv[])
{
    struct strange xp[3];	//结构体数组 
    int characters[6] = "abcde";		//虽然编译错误，但是有3种猜测
										//1."abcde"是一个地址，地址给[0]了
										//2.根据小端存储，存[0]和[1]了
										//3.每个一个
										//通过思考char *s[6]的行为，我分析应该是2 
										//这个时候小端就是s[0] ='d''c''b''a',s[1]='e''0''0''0',后面都是0 
										//（补充一下，其实有个小问题，如果是按照（int *）转，那我就全错了） 
    for (int i = 0; i < 6; i++) {
        *((char *)(xp + 0) + i) = characters[i];	//偏移量每次加一，正好把结构体xp[0]的赋值了 
    }					//d c b a e 0 

    strcpy((char *)&xp[1], (char *)characters);//记得以前写过strcpy的代码，就是相当于强转（int *）为（char *）
												//d c b a e 0
    memcpy(&xp[2], characters, sizeof(struct strange));//这个是内存拷贝，能考多少考多少 
    printf("%zu\n", strlen((char *)characters));

    for (int i = 0; i < 3; i++) {
        printf("%c\n", xp[i].e);		//都是e 
    }
}
