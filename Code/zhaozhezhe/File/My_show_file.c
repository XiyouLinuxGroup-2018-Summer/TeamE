#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#if 0
 	struct dirent {
               ino_t          d_ino;       /* Inode number */
               off_t          d_off;       /* Not an offset; see below */
               unsigned short d_reclen;    /* Length of this record */
               unsigned char  d_type;      /* Type of file; not supported
                                              by all filesystem types */
               char           d_name[256]; /* Null-terminated filename */
         };

#endif
int main(void){

	DIR *Dir;
	struct dirent *dirent;

	Dir = opendir(".");

	while((dirent = readdir(Dir)) != NULL){
		printf("%s\n", /*段错误dirent->d_ino, dirent->d_off, dirent->d_reclen, dirent->d_type, */dirent->d_name);
	}


}
