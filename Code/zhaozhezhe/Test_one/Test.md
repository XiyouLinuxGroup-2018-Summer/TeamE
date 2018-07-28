#### 1.

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tag_unimportant {
    char *name;
    int num;
} A;
struct tag_unimportant *func(A *a)
{
    a = (A *)malloc(sizeof(A));
    a->name = (char *)malloc(sizeof("Xiyou Linux Group"));
    strcpy(a->name, "Xiyou Linux Group");
    a->num = 20180728;
    return a;
}

int main(int argc, char *argv[])
{
    A *a;
    a = func(a);
    printf("%s %d\n", a->name, a->num);
    return 0;
}
```



- 结构体成员`char *name`未指向一个正确的内存，而是一个野指针。
- `A *a`也未指向一个有效的内存也是一个野指针。
- 所以函数func需返回一个指针指向a。`name`成员也要一个指向。

#### 2.

```C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int convert(const char *num){
    int i,j;
    int Num = 0;
    for(i=0; i<strlen(num);i++){
    	//printf("%c", num[i]);
        int len = 1;
        for(j=strlen(num)-i-2; j>=0; j--){
        	len *= 10;
        }
    	Num += (num[i] - '0')*len;
    }
    
    return Num;
}

int main(void){

	char *num = "7843945";
	int a = convert(num);
	printf("%d\n",a);
	return 0;
}
```

#### 3

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct strange {
    char a, b, c, d, e, zero;
};

int main(int argc, char *argv[])
{
    //申请一个结构体数组。
    struct strange xp[3];
    //申请一个字符串数组
    char characters[6] = "abcde";
    //对struct strage[0]进行填充，(char *)(xp + 0)指向结构体[0]的的一个成员a，依次加。
    for (int i = 0; i < 6; i++) {
        *((char *)(xp + 0) + i) = characters[i];
    }
    
	//将结构体[1]强制类型转换为char *型的指针，指向地址首部，向里依次填充abcde。
    strcpy((char *)&xp[1], (char *)characters);
    //拷贝数组，将character赋值给结构体[3].
    memcpy(&xp[2], characters, sizeof(struct strange));
    // 不懂zu。字符串长度
    printf("%zu\n", strlen((char *)characters));

    //输出 三个e
    for (int i = 0; i < 3; i++) {
        printf("%c\n", xp[i].e);
    }
}
```

#### 4

