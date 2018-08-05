1. 简要解释该语句的输出:`printf("%d\n", &(&4[(int *)3])[5]);`

   答：输出从0地址开始，到10个int长度的地址尾长度`39` ，`(&4[(int *)3])`将 3`强制转换为int *的指针`，并指向3这个地址，从0地址开始行有移动4个(int *)指针的长度 所以为19，`&(&4[(int *)3])[5])`向右再偏移5个int单位的偏移量指向`39`

2. 分析代码并说明:
   a. 最终输出多少次 "hello, world?";

   8次`hello,world?`.

   b. 最终将产生共多少个进程。

   ```C
   int main(int argc, char *argv[]){
       for (int i = 0; i < 2; i++){
           pid_t pid = fork();
           printf("hello, world?");
       }
       return 0;
   }
   //两次执行fork,第一次是fork一个子进程，子进程和父进程分别打印一个hello,world?
   //第二次执行fork。父进程和子进程分别fork一个新的子进程。原先的父进程和子进程。加上新开的两个进程，又分别打印一个hello,world?。
   printf("hello,world?\n");//将上面的打印语句换成这个之后，可以看出少了两个hello,world?
   //。。。。。。。。。。。。。应该是输出缓冲区的问题，缓冲区没有将hello,world?及时的缓冲出去。
   ```

   

3. 简要说明 exec 系列函数。

   `exec`系列函数是可以调用外部可执行程序的一个函数，调用该函数时，原进程除了进程ID被`exec`调用的可执行程序继承之外，除此之外原进程的所有不保留。

4. 以下代码的输出是?若该程序名为:test,请分析并解释执行 test | cat 后
  的结果。

  ```C
  int main(int argc, char *argv[]){
      
      printf("Hello, my pid is %d\n", getpid());
      
      pid_t pid = fork();
      if (pid == 0)
      {
          printf("I was forked! :D %d\n", getpid());
      }
      else
      {
      	waitpid(pid, NULL, 0);
      	printf("%d was forked!\n", pid);
      }
      
      return 0;
  }
  
  ```

  **输出为**

  `Hello, my pid is 父进程pid`

  `I was forked! :D 子进程pid`

  `子进程pid was forked!`

  **执行./test | cat后的结果。**

  `Hello, my pid is 父进程pid`

  `I was forked! :D 子进程pid`

  `Hello, my pid is 父进程pid`

  `子进程pid was forked!`

  子进程复制父进程的所有之后，没有打印`hello`，cat函数帮助打印

