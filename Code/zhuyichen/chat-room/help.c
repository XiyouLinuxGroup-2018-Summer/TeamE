#include "chr.h" 

void help(int sockfd)
{
    static char buf[] = {
       "========================================================================\n"
       ">  1.login                    [usage:  <1\\n> <username password\\n>]    <\n"
       ">  2.register                 [usage:  <2\\n> <username password\\n>]    <\n"
       ">  3.change password          [usage:  <3\\n> <userid newpassword\\n>]   <\n"
       ">  4.add friend               [usage:  <4\\n> <friendname\\n>]           <\n"
       ">  5.del friend               [usage:  <5\\n> <friendname\\n>]           <\n"
       ">  6.private chat             [usage:  <6\\n> <friendname\\n>]           <\n"
       ">  7.group chat               [usage:  <7\\n> <groupname\\n>]            <\n"
       ">  8.view chatting records    [usage:  <8\\n> <name\\n>]                 <\n"
       "> 10.add group                [usage: <10\\n> <groupname\\n>]            <\n"
       "> 11.creat group              [usage: <11\\n> <groupname\\n>]            <\n"
       "> 12.leave group              [usage: <12\\n> <groupname\\n>]            <\n"
       "> 13.set administrator        [usage: <13\\n> <groupmember\\n>]          <\n"
       "========================================================================\n"
       ">'\\h': show service list                                               <\n"
       ">'\\c': refresh screen                                                  <\n"
       ">'\\q': cancel a service <1~13 above>                                   <\n"
       "========================================================================\n"
    };
    Writen(sockfd, buf, strlen(buf));
}

void clean(int sockfd)
{
    char buf[] = "\033c";
    Writen(sockfd, buf, strlen(buf));
}

