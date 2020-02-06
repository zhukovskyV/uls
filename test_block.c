// #include <dirent.h>
#include <stdio.h>
// #include <time.h>
// #include <sys/stat.h>
#include <stdlib.h>
// #include <unistd.h>
// #include <pwd.h>
#include <string.h>
// #include <sys/types.h>
// #include <sys/xattr.h>
// #include <sys/acl.h>

typedef struct s_len_column {
    int len_prav;
    int len_link;
    int len_user;
    int len_gid;
    int len_size;
    int len_time;
}               t_len_column;


void filling_struct(t_len_column *lens) {
    memset(lens, 0, sizeof(t_len_column));
}

int main() {
    t_len_column *p = (t_len_column *)malloc(sizeof(t_len_column));
    filling_struct(p);
    printf("len_prav = %d\n", p->len_prav);
    printf("len_link = %d\n", p->len_link);
    printf("len_user = %d\n", p->len_user);
    printf("len_gid = %d\n", p->len_gid );
    printf("len_size = %d\n", p->len_size);
    printf("len_time = %d\n", p->len_time);
}