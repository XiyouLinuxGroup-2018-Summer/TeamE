/*********************第一题原始代码*********************/
typedef struct tag_unimportant {
    char *name;
    int num;
} A;

void func(A *a)
{
    a = (A *)malloc(sizeof(A));
    strcpy(a->name, "Xiyou Linux Group");   /*  没有为name成员分配内存  */
    a->num = 20180728;
}

int main(int argc, char *argv[])
{
    A *a;
    func(a);   /*  C中参数传递是传值调用  */
    printf("%s %d\n", a->name, a->num);
    return 0;
}
/*********************修改******************************/
void func(A**a)
{
    *a = (A *)malloc(sizeof(A));
    (*a)->name = strdup("Xiyou Linux Group");
    (*a)->num = 20180728;
}
/*********************第二题*****************************/
int convert(const char *num)
{
    int val;
    int sign;

    while (isspace(*num))
        num++;
    sign = (*num == '-') ? -1 : 1;
    for (val = 0; isdigit(*num); num++)
        val = val * 10 + (*num - '0');
    return val * sign;
}
/*********************第三题原始代码*********************/
struct strange {
    char a, b, c, d, e, zero;
};

int main(int argc, char *argv[])
{
    struct strange xp[3];   /*  定义了一个长度为3的结构体数组  */
    int characters[6] = "abcde";    /*  定义了一个字符数组  */
    for (int i = 0; i < 6; i++) {
        *((char *)(xp + 0) + i) = characters[i];
    }   /*  将字符串“abced"拷贝到xp[0]的所有成员中  */

    strcpy((char *)&xp[1], (char *)characters);   /*  将“abcde"拷贝到xp[1]的所有成员中  */
    memcpy(&xp[2], characters, sizeof(struct strange));  /*  将“abcde“拷贝到xp[2]的所有成员中  */
    printf("%zu\n", strlen((char *)characters));   /*  长度为6  */

    for (int i = 0; i < 3; i++) {
        printf("%c\n", xp[i].e);  /*  结果为'\0'  */
    }
}
