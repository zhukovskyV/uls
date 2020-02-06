#ifndef ULS_H
#define ULS_H

#include "libmx/inc/libmx.h"

#include <sys/acl.h>
#include <sys/xattr.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/acl.h>
#include <sys/types.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include <wchar.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


typedef struct s_len_column {
	int len_prav;
	int	len_link;
	int len_user;
	int len_gid;
	int len_size;
	int len_time;
}				t_len_column;

typedef struct s_diff_len {
	// int diff_prav;
	int	diff_link;
	int diff_user;
	// int diff_gid;
	int diff_size;
	// int diff_time;
}				t_diff_len;

typedef struct s_flag {
	bool flag_l;
	bool flag_a;
	bool flag_A;	
}				t_flag;

// typedef struct s_all_info {
// 	int ac;
// 	char **av;

// 	bool flags_exist;
// 	bool file_exist;
// 	bool folder_exist;

// }				t_all_info;

// typedef struct s_in_folder{
	
// }				t_in_folder;

// typedef struct s_folder {
// 	char *name;
// 	int num_of_sub;
// 	int max_sub_len;
	
// }				t_folder;

char *mx_mychmod(int mode, char *str, char *filename);
void mx_print_spaces(int n);
char* mx_substr(const char *src, int a, int b);
int mx_intlen(long long n);

#endif
