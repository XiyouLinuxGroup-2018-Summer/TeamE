#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <mysql/mysql.h>
#include "cJSON.h"
#include <openssl/md5.h>
#include <iostream>
using namespace std;


MYSQL mysqld;       //mysql句柄
int aqq[10] = { 0 };
struct yonghuzaixianzhuangtai
{
    int socket;
    int id;
    //char name[20];
};
int yonghuzaixina = 0;
yonghuzaixianzhuangtai yonghuzaixian[1000] = { 0 };

struct add_friend_waits
{
    int id1;        //1是自己的
    int id2;        //2是朋友的id
};
int add_friend_wati = 0;
add_friend_waits add_friend_wait[100] = { 0 };
//struct yonghuxinxi
//{
//    char *id;
//    char *name;
//    char *age;
//    char *passwd;
//    char *style;
//    char *namehead;
//};
char *yonghuxinxi[1][6];

struct zhuceyonghu      //注册用户
{
public:
    int weiyibiaoshifu;
    char *yonghuming;
    char *yongbumima;
};
struct xiugaimima       //修改密码
{
public:
    int weiyibiaoshifu;     //2
    char *yongbumima;
    char *yonghumishi;
};
struct willlogin
{
public:
    int weiyibiaoshifu;     //3
    char *yonghuming;
    char *yongbumima;
};
struct haoyouyonghuxinxi
{
    int weiyibiaoshifu;
    char *id;
    char *name;
    char *age;
    char *style;
    char *namehead;
};
struct adduser2             //15
{
    int idme;
    int weiyibiaoshifu;
    int id_want_to_add;
};

void setnonblocking(int st) //非阻塞
{
    int a = fcntl(st,F_GETFL);
    if(a < 0)
    {printf("change fcntl error\n");return;}
    a = a | O_NONBLOCK;
    if(fcntl(st,F_SETFL,a) < 0)
    {
        printf("set error\n");
        return;
    }
    return;
}
int socket_accept(int st)
{
    int client_st = 0;		//clientsocket
    struct sockaddr_in client_addr; 	//client
    memset(&client_addr,0,sizeof(client_addr));
    socklen_t len = sizeof(client_addr);
    client_st = accept(st,(struct sockaddr *)&client_addr,&len);
    if(client_st == -1)
    {
        printf("accept error,%s\n",strerror(errno));
        return 0;
    }
    else
        printf("accept by %s\n",inet_ntoa(client_addr.sin_addr));
    return client_st;
}

int comeinswitch1(cJSON *p,int st)
{
    char b[105];
    char c[105];
    strcpy(b,cJSON_GetObjectItem(p,"yonghuming")->valuestring);
    strcpy(c,cJSON_GetObjectItem(p,"yongbumima")->valuestring);
    char sql[200];
    sprintf(sql,"INSERT INTO table3(name,passwd) VALUES('%s','%s');",b,c);
    if(0 != mysql_real_query(&mysqld,sql,strlen(sql)))
    {
        printf("insert mysql ok\n");
    }
    memset(sql,0,sizeof(sql));
    sprintf(sql,"SELECT ID FROM table3 WHERE name LIKE '%s';",b);
    printf("sql = %s",sql);
    if(0 != mysql_real_query(&mysqld,sql,strlen(sql)))
    {
        printf("mysql error\n");
        return 0;
    }
    else
        printf("mysql select ok!");
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(&mysqld);
    int myid;
    while((row = mysql_fetch_row(res)) != NULL)
    {
        int t;
        for(t = 0;t < mysql_num_fields(res);t++)
        {
            printf("%s\n",row[t]);
            myid = atoi(row[t]);
        }
    }
    send(st,&myid,4,0);
}

int comeinswitch2(cJSON *p,int st)
{
    char b[105];
    char c[105];
    strcpy(b,cJSON_GetObjectItem(p,"yonghumishi")->valuestring);
    strcpy(c,cJSON_GetObjectItem(p,"yongbumima")->valuestring);
    printf("yongbumima = %s\n",c);
    char sql[200];
    sprintf(sql,"UPDATE table3 SET passwd = '%s' WHERE id = %d;",c,atoi(b));
    if(0 == mysql_real_query(&mysqld,sql,strlen(sql)))
    {
        printf("change mysql ok\n");
        int a = 1;
        send(st,&a,4,0);
        return 0;
    }
    int a = 0;
    send(st,&a,4,0);
    return 0;
}
int comeinswitch3(cJSON *p,int st)
{
    char b[105] = { 0 };
    char c[105] = { 0 };
    strcpy(b,cJSON_GetObjectItem(p,"yonghuming")->valuestring);
    strcpy(c,cJSON_GetObjectItem(p,"yongbumima")->valuestring);
    printf("yonghuID = %s yongbumima = %s\n",b,c);
    char sql[200] = { 0 };
    sprintf(sql,"SELECT * FROM table3 WHERE ID = %d AND passwd = '%s';",atoi(b),c);
    if(0 != mysql_real_query(&mysqld,sql,strlen(sql)))
    {
        printf("find mysql ok\n");
    }
    printf("sql = %s\n",sql);

    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(&mysqld);

    //在这顺便读取用户数据
    memset(yonghuxinxi,0,sizeof(yonghuxinxi));
    int yonghushu = 0;
    while(row = mysql_fetch_row(res))
    {
        int t;
        for(t = 0;t < mysql_num_fields(res);t++)
        {
            if(row[t] == NULL)
            {
                yonghushu++;
                continue;
            }
            yonghuxinxi[0][yonghushu] = (char *)malloc(strlen(row[t]));
            strcpy(yonghuxinxi[0][yonghushu++],(const char *)row[t]);
        }
    }
    int abccc = 0;
    for(abccc = 0;abccc < 6;abccc++)
    {
        if(yonghuxinxi[0][abccc])
            printf("%s\n",yonghuxinxi[0][abccc]);
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    int flagsss = 1;
    if(strlen(yonghuxinxi[0][0]) > 0)
    {
        yonghuzaixianzhuangtai *t1 = (yonghuzaixianzhuangtai *)calloc(sizeof(yonghuzaixianzhuangtai),1);
        t1->socket = st;
        t1->id = atoi(b);
        int ii = 0;
        for(ii = 0;ii < yonghuzaixina;ii++)
        {
            if(yonghuzaixian[ii].id == t1->id)
            {
                int a = 0;
                send(st,&a,4,0);
                return 0;
            }
        }
        yonghuzaixian[yonghuzaixina].id = t1->id;
        yonghuzaixian[yonghuzaixina].socket = t1->socket;
        yonghuzaixina++;
        //yonghuzaixian[yonghuzaixina++] = t1;
        printf("yonghuzaixianma = %d\n",yonghuzaixina);
    }
    else
    {

        int a = 2;
        send(st,&a,4,0);
        return 0;
    }
    int a = 1;
    send(st,&a,4,0);
    //发送用户的数据

    struct yonghuxinxi11
    {
        char *a;
        char *b;
        char *c;
        char *d;
        char *e;
        char *f;
    };
    yonghuxinxi11 aaaa11;
    memset(&aaaa11,0,sizeof (aaaa11));
    aaaa11.a = yonghuxinxi[0][0];
    aaaa11.b = yonghuxinxi[0][1];
    aaaa11.c = yonghuxinxi[0][2];
    aaaa11.d = yonghuxinxi[0][3];
    aaaa11.e = yonghuxinxi[0][4];
    aaaa11.f = yonghuxinxi[0][5];


    cJSON *yonghuxinxi = cJSON_CreateObject();
    if(aaaa11.a)
        cJSON_AddStringToObject(yonghuxinxi,"0",aaaa11.a);
    if(aaaa11.b)
        cJSON_AddStringToObject(yonghuxinxi,"1",aaaa11.b);
    if(aaaa11.c)
        cJSON_AddStringToObject(yonghuxinxi,"2",aaaa11.c);
    if(aaaa11.d)
        cJSON_AddStringToObject(yonghuxinxi,"3",aaaa11.d);
    if(aaaa11.e)
        cJSON_AddStringToObject(yonghuxinxi,"4",aaaa11.e);
    if(aaaa11.f)
        cJSON_AddStringToObject(yonghuxinxi,"5",aaaa11.f);

    char *out = cJSON_Print(yonghuxinxi);
    printf("%s\n",out);
    out = cJSON_PrintUnformatted(yonghuxinxi);
    int strlenss = strlen(out) + 1;
    send(st,&strlenss,4,0);
    send(st,out,strlenss,0);
    printf("send success\n");
    //发送用户数据end

    //发送好友信息
    memset(sql,0,sizeof(sql));
    //select ID,name,AGE,style,namehead from table3 where ID = (select id2 from table5 where id1 = %d);
    sprintf(sql,"select id2 from table5 where id1 = %d;",atoi(b));
    if(0 != mysql_real_query(&mysqld,sql,strlen(sql)))
    {
        printf("find mysql ok\n");
    }


    memset(res,0,sizeof(res));
    memset(&row,0,sizeof(row));
    res = mysql_store_result(&mysqld);


    int Get_friend[200] = { 0 };
    int Get_friendnum = 0;


    while(row = mysql_fetch_row(res))
    {
        int t;
        for(t = 0;t < mysql_num_fields(res);t++)
        {
            if(row[t] == NULL)
            {
                continue;
            }
            Get_friend[Get_friendnum++] = atoi(row[t]);
        }
    }
    int ii1 = 0;
    for(;ii1 < Get_friendnum;ii1++)
    {
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~%d\n",Get_friend[ii1]);
    }

    //在这顺便读取用户数据
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~@@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    memset(yonghuxinxi,0,sizeof(yonghuxinxi));
    for(ii1 = 0;ii1 < Get_friendnum;ii1++)
    {
        memset(sql,0,sizeof(sql));
        sprintf(sql,"select ID,name,AGE,style,namehead from table3 where ID = %d;",Get_friend[ii1]);
        if(0 != mysql_real_query(&mysqld,sql,strlen(sql)))
        {
            printf("find mysql ok\n");
        }
        memset(res,0,sizeof(res));
        memset(&row,0,sizeof(row));
        res = mysql_store_result(&mysqld);
        while((row = mysql_fetch_row(res)))
        {
            int t;
            haoyouyonghuxinxi haoyou;
            memset(&haoyou,0,sizeof(haoyou));
            for(t = 0;t < mysql_num_fields(res);t++)
            {
                if(row[t] == NULL)
                {
                    yonghushu++;
                    continue;
                }
                if(t == 0)
                {
                    haoyou.id = row[t];
                }
                if(t == 1)
                {
                    haoyou.name = row[t];
                }
                if(t == 2)
                {
                    haoyou.age = row[t];
                }
                if(t == 3)
                {
                    haoyou.style = row[t];
                }
                if(t == 4)
                {
                    haoyou.namehead = row[t];
                }
            }
            haoyou.weiyibiaoshifu = 1;
            cJSON *haoyouxinxi = cJSON_CreateObject();
            //        char *id;
            //        char *name;
            //        char *age;
            //        char *style;
            //        char *namehead;
            cJSON_AddNumberToObject(haoyouxinxi,"weiyibiaoshifu",haoyou.weiyibiaoshifu);
            if(haoyou.age)
                cJSON_AddStringToObject(haoyouxinxi,"age",haoyou.age);
            if(haoyou.id)
                cJSON_AddStringToObject(haoyouxinxi,"id",haoyou.id);
            if(haoyou.name)
                cJSON_AddStringToObject(haoyouxinxi,"name",haoyou.name);
            if(haoyou.namehead)
                cJSON_AddStringToObject(haoyouxinxi,"namehead",haoyou.namehead);
            if(haoyou.style)
                cJSON_AddStringToObject(haoyouxinxi,"style",haoyou.style);
            char *out = cJSON_Print(haoyouxinxi);
            printf("%s\n",out);
            //texteditusername11->setText(out);
            out = cJSON_PrintUnformatted(haoyouxinxi);
            int strlenss = strlen(out) + 1;
            send(st,&strlenss,4,0);
            send(st,out,strlenss,0);
            free(out);
            cJSON_Delete(haoyouxinxi);
            printf("send success\n");
        }
    }
    printf("```````````````````````````````````````````````````````````have information!\n");


    //-------------------这里要复用很多东西


    int jjaaa = 0;
    struct send_add_friend
    {
        int weiyibiaoshifu;     //2
        int send_id;           //通过这个就可以知道你想要发给谁
        int num;                //这个是返回值
    };
    for(jjaaa = 0;jjaaa < add_friend_wati;jjaaa++)
    {
        //id2是别人想要加的人，也就是你！
        if(add_friend_wait[jjaaa].id2 == atoi(b))
        {
            printf("```````````````````````````````````````````````````````````have information!\n");
            send_add_friend add1;
            add1.weiyibiaoshifu = 2;
            add1.send_id = add_friend_wait[jjaaa].id1;
            add1.num = 10;
            cJSON *yonghuxinxi = cJSON_CreateObject();
            cJSON_AddNumberToObject(yonghuxinxi,"weiyibiaoshifu",add1.weiyibiaoshifu);
            cJSON_AddNumberToObject(yonghuxinxi,"send_id",add1.send_id);
            cJSON_AddNumberToObject(yonghuxinxi,"num",add1.num);

            char *out = cJSON_Print(yonghuxinxi);
            printf("%s\n",out);
            out = cJSON_PrintUnformatted(yonghuxinxi);
            int strlenss = strlen(out) + 1;
            send(st,&strlenss,4,0);
            send(st,out,strlenss,0);
            return 0;
        }
    }



    return 0;
}

int comeinswitch15(cJSON *p,int st)
{
    adduser2 add_friend;
    memset(&add_friend,0,sizeof(add_friend));
    add_friend.idme = cJSON_GetObjectItem(p,"idme")->valueint;
    add_friend.id_want_to_add = cJSON_GetObjectItem(p,"id_want_to_add")->valueint;
    printf("add_friend    %d    %d\n",add_friend.idme,add_friend.id_want_to_add);
    char sql[200];
    sprintf(sql,"SELECT id FROM table3 WHERE id = %d;",add_friend.id_want_to_add);
    if(0 == mysql_real_query(&mysqld,sql,strlen(sql)))
    {
        printf("change mysql ok\n");
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(&mysqld);
    int myid = 0;
    while((row = mysql_fetch_row(res)) != NULL)
    {
        int t;
        for(t = 0;t < mysql_num_fields(res);t++)
        {
            printf("%s\n",row[t]);
            myid = atoi(row[t]);
        }
    }
    struct send_add_friend
    {
        int weiyibiaoshifu;     //2
        int send_id;           //通过这个就可以知道你想要发给谁
        int num;                //这个是返回值
    };

    if(myid)
    {
        int i = 0;
        for(;i < add_friend_wati;i++)       //如果都已经发过了，就不用再发了
        {
            if((add_friend_wait[i].id1 == add_friend.idme) && (add_friend_wait[i].id2 == myid))
            {
                return 0;
            }
        }
        for(i = 0;i < yonghuzaixina;i++)
        {
            if(yonghuzaixian[i].id == myid)
            {
                printf("想要添加的好友在线\n");
                send_add_friend add1;
                add1.weiyibiaoshifu = 2;
                add1.send_id = add_friend.idme;
                add1.num = 10;
                cJSON *yonghuxinxi = cJSON_CreateObject();
                cJSON_AddNumberToObject(yonghuxinxi,"weiyibiaoshifu",add1.weiyibiaoshifu);
                cJSON_AddNumberToObject(yonghuxinxi,"send_id",add1.send_id);
                cJSON_AddNumberToObject(yonghuxinxi,"num",add1.num);

                char *out = cJSON_Print(yonghuxinxi);
                printf("%s\n",out);
                out = cJSON_PrintUnformatted(yonghuxinxi);
                int strlenss = strlen(out) + 1;
                send(yonghuzaixian[i].socket,&strlenss,4,0);
                send(yonghuzaixian[i].socket,out,strlenss,0);
                free(out);
                cJSON_Delete(yonghuxinxi);
                return 0;
            }
        }
        add_friend_wait[add_friend_wati].id1 = add_friend.idme;
        add_friend_wait[add_friend_wati].id2 = myid;
        add_friend_wati++;
    }
    else                        //都没有用户，肯定是返回喽
    {
        send_add_friend add1;
        add1.weiyibiaoshifu = 2;
        add1.send_id = add_friend.idme;
        add1.num = 1;          //num是1是察无此人
        cJSON *yonghuxinxi = cJSON_CreateObject();
        cJSON_AddNumberToObject(yonghuxinxi,"weiyibiaoshifu",add1.weiyibiaoshifu);
        cJSON_AddNumberToObject(yonghuxinxi,"send_id",add1.send_id);
        cJSON_AddNumberToObject(yonghuxinxi,"num",add1.num);

        char *out = cJSON_Print(yonghuxinxi);
        printf("%s\n",out);
        out = cJSON_PrintUnformatted(yonghuxinxi);
        int strlenss = strlen(out) + 1;
        send(st,&strlenss,4,0);
        send(st,out,strlenss,0);
        return 0;
    }

}

int comeinswitch16(cJSON *p,int st)
{
    struct recv_add_friend
    {
        int weiyibiaoshifu;
        int myid;
        int herid;
    };
    recv_add_friend abc;
    memset(&abc,0,sizeof(abc));
    abc.myid = cJSON_GetObjectItem(p,"myid")->valueint;
    abc.herid = cJSON_GetObjectItem(p,"herid")->valueint;
    printf("16 - recv_add_friend    %d    %d\n",abc.myid,abc.herid);
    char sql[200];
    sprintf(sql,"INSERT INTO table5(id1,id2) VALUES(%d,%d);",abc.myid,abc.herid);
    if(0 == mysql_real_query(&mysqld,sql,strlen(sql)))
    {
        printf("change mysql ok\n");
    }
    memset(&sql,0,sizeof(sql));
    sprintf(sql,"INSERT INTO table5(id1,id2) VALUES(%d,%d);",abc.herid,abc.myid);
    if(0 == mysql_real_query(&mysqld,sql,strlen(sql)))
    {
        printf("change mysql ok\n");
    }

    //--->之后还得更新下
}

int main()
{
    //开始就把mysql连接好
    mysql_init(&mysqld);
    if(!mysql_real_connect(&mysqld,"localhost","dbuser1","dbuser1","db1",0,NULL,0))
    {
        printf("Error to connecting to mysql!\n");
    }
    else
    {
        printf("Connected Mysql success\n");
    }
    mysql_real_query(&mysqld,"SET NAMES utf8;",strlen("SET NAMES utf8;"));
    memset(aqq,0,sizeof(aqq));
    int st = socket(AF_INET,SOCK_STREAM,0);

    int on = 1;
    if(setsockopt(st,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) == -1)
    {
        printf("setsocopt failed %s\n",strerror(errno));
        return 0;
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(st,(struct sockaddr *)&addr,sizeof(addr)) == -1)
    {
        printf("bin failed %s\n",strerror(errno));
        return 0;
    }
    if(listen(st,1000) == -1)
    {
        printf("listen failed %s\n",strerror(errno));
        return 0;
    }
    printf("st = %d",st);	//这个是看开始的st是多少
    struct epoll_event ev ,events[100];
    int epfd = epoll_create(100);
    setnonblocking(st);
    ev.data.fd = st;
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    //ev.events=EPOLLIN|EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,st,&ev);
    int sst  = 0;
    pthread_t pthr;
    while(1)
    {
        int nfds = epoll_wait(epfd,events,100,-1);
        if(nfds == -1)
        {
            printf("epoll_wait falied\n");
            break;
        }
        int i;
        for(i = 0;i < nfds;i++)
        {
            if(events[i].data.fd < 0)
                continue;
            if(events[i].data.fd == st)
            {
                sst = socket_accept(st);
                if(sst > 0)
                {
                    setnonblocking(st);
                    ev.data.fd = sst;
                    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,sst,&ev);
                    continue;
                }

            }
            if(events[i].events & EPOLLERR) //socket
            {
                sst = events[i].data.fd;
                close(sst);
                epoll_ctl(epfd,EPOLL_CTL_DEL,sst,NULL);
                continue;
            }
            if(events[i].events & EPOLLHUP)
            {
                sst = events[i].data.fd;
                close(sst);
                epoll_ctl(epfd,EPOLL_CTL_DEL,sst,NULL);
                continue;
            }
            char *buf1 = NULL;
            if(events[i].events & EPOLLIN)
            {
                sst = events[i].data.fd;
                if(sst <= 0)
                {
                    //Yonghushifouzaixian.remove();
                    close(sst);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,sst,NULL);
                    continue;
                }
                int *size = (int *)malloc(sizeof(int));
                int rcm = 0;
                int sheng = 4;
                int aaabbbccc = 4;
                int shengi = 5;
                while(rcm < 4)
                {
                    rcm += recv(sst,size,sheng,0);
                    sheng = aaabbbccc - rcm;
                    shengi--;
                    if(shengi == 0)
                        break;
                }
                if(rcm == 0)
                {
                    printf("%s",strerror(errno));
                    yonghuzaixianzhuangtai *p = NULL;
                    int ii = 1;
                    int flag = -1;
                    int abcc11 = 0;
                    for(ii = 0;ii < yonghuzaixina;ii++)
                    {
                        if(sst == yonghuzaixian[ii].socket)
                        {
                            flag = ii;
                        }
                    }
                    if(flag == -1)
                    {
                        printf("这个没有上去，不用删除\n");
                        epoll_ctl(epfd,EPOLL_CTL_DEL,sst,NULL);
                        continue;
                    }
                    for(ii = flag;ii < yonghuzaixina - 1;ii++)
                    {
                        yonghuzaixian[ii] = yonghuzaixian[ii + 1];
                    }
                    yonghuzaixina--;
                    printf("flag = %d,yonghuzaixina = %d\n",flag,yonghuzaixina);
                    memset(&yonghuzaixian[yonghuzaixina],0,sizeof(int *));
                    //~~~~~~~~~~~~~~~~~~~~~~~~~@登陆这里还得扩充！为以后别人看好友状态作铺垫@~~~~~~~~~~~~~~~~~~~~~~//

                    epoll_ctl(epfd,EPOLL_CTL_DEL,sst,NULL);
                    continue;
                }
                rcm = 0;
                sheng = *size;
                buf1 = (char *)malloc(*size);
                while(rcm < *size)
                {
                    rcm += recv(sst,&buf1[rcm],sheng,0);
                    sheng = *size - rcm;
                }
                printf("buf1 = %s\n",buf1);

            }
            if(buf1 == NULL)
                continue;
            cJSON *CONGZHELIKAISHI = cJSON_Parse(buf1);
            free(buf1);         //注册用户之后不用buf1，只用cJSON就成
            //printf("out = %s",out);
            int thissa = cJSON_GetObjectItem(CONGZHELIKAISHI,"weiyibiaoshifu")->valueint;
            printf("thissa = %d\n",thissa);
            switch (thissa)
            {
            case 1:     //注册用户
            {
                comeinswitch1(CONGZHELIKAISHI,sst);
                break;
            }
            case 2:     //修改密码
            {
                comeinswitch2(CONGZHELIKAISHI,sst);
                break;
            }
            case 3:     //用户登陆
            {
                comeinswitch3(CONGZHELIKAISHI,sst);
                break;
            }
            case 4:
            {
                break;
            }
            case 5:
            {
                break;
            }
            case 6:
            {
                break;
            }
            case 7:
            {
                break;
            }
            case 8:
            {
                break;
            }
            case 9:
            {
                break;
            }
            case 10:
            {
                break;
            }
            case 11:
            {
                break;
            }
            case 12:
            {
                break;
            }
            case 13:
            {
                break;
            }
            case 14:
            {
                break;
            }
            case 15:
            {
                comeinswitch15(CONGZHELIKAISHI,sst);
                break;
            }
            case 16:
            {
                comeinswitch16(CONGZHELIKAISHI,sst);
                break;
            }



            }
        }
    }
    printf("111");
    return 0;
}

