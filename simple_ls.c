#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/acl.h>

void xattr(char *str) {
    acl_t acl = NULL;
    ssize_t xattr = 0;
    char *filename = ".";

    xattr = listxattr(filename, NULL, 0, XATTR_NOFOLLOW);
    if (xattr < 0)
        xattr = 0;

    if (xattr > 0)
        str[10] = '@';
    else if (acl != NULL)
        str[10] = '+';
    else
        str[10] = ' ';
}

char *my_chmode(int mode, char *str) {
    strcpy(str,"----------");

    if(S_ISDIR(mode))str[0]='d';
    if(S_ISCHR(mode))str[0]='c';
    if(S_ISBLK(mode))str[0]='b';
    if(mode & S_IRUSR)str[1]='r';
    if(mode & S_IWUSR)str[2]='w';
    if(mode & S_IXUSR)str[3]='x';
    if(mode & S_IRGRP)str[4]='r';
    if(mode & S_IWGRP)str[5]='w';
    if(mode & S_IXGRP)str[6]='x';
    if(mode & S_IROTH)str[7]='r';
    if(mode & S_IWOTH)str[8]='w';
    if(mode & S_IXOTH)str[9]='x';
        
    return str;
}



char *formatdate(char *s, time_t val) {
        strftime(s, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return s;
}

void my_time(char *filename) {
    int errno = 0;
    
    struct stat *file_info = malloc(sizeof(struct stat));

    lstat(filename, file_info);
    char time_info[36]; // хз пока сколько выделять
    // printf(/*"Access: */"%s  ", formatdate(time_info, file_info->st_atime));
    printf("Modify: %s | ", ctime(&(file_info->st_mtime)));
    // printf("Change: %s | ", formatdate(time_info, file_info->st_ctime));
    // printf("Birth: %s | ", formatdate(time_info, file_info->st_birthtime));
    free(file_info);
}

// long long total(long long num,) {
//     long long i = 0;

//     i += num;
// }
int main(int ac, char **av) {
    char pr_dost[11];
    DIR *dp;
    FILE *fp;
    struct dirent *dirp;
    struct stat buff; // структура с инфой по файлу
    struct passwd *user;

    user = getpwuid(getuid()/*buff.st_uid*/);
    if (ac != 2) {
        printf("че, ахуел сука?!");
        exit(0);
    }
    if ((dp = opendir(av[1])) == NULL) {
        printf("error %s\n", av[1]);
        exit(0);
    }
    printf("total\n");
    while ((dirp = readdir(dp)) != NULL) {
        stat(dirp->d_name, &buff);
        // printf("%s  ", my_chmode(buff.st_mode, pr_dost));
        // printf("%d  ", buff.st_nlink);
        // printf("%s  ", user->pw_name);
        // printf("%u  ", buff.st_gid);
        // printf("%lld  ", buff.st_size);
        printf("%lld  ", buff.st_blocks);
        my_time(dirp->d_name);
        // printf("%.12s  ",4+ctime(&(buff.st_mtime)));
        printf("%s\n", dirp->d_name);
    }
    closedir(dp);
    exit(0);
}
