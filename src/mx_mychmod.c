#include "uls.h"

char *mx_mychmod(int mode, char *str, char *dir_mame) {
    acl_t acl = NULL;
    // acl_entry_t enter;
    ssize_t xattr = 0;

    mx_strcpy(str,"-----------");
    // acl = acl_get_link_np(dir_mame, ACL_TYPE_EXTENDED);
    // if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &enter) == -1) {
    //     acl_free(acl);
    //     acl = NULL;
    // }

    xattr = listxattr(dir_mame, NULL, 0, XATTR_NOFOLLOW);
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
    if (xattr < 0)
        xattr = 0;
    if (xattr > 0)
        str[10] = '@';
    else if (acl != NULL)
        str[10] = '+';
    else
        str[10] = ' ';
    return str;
}
