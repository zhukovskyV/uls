#include "uls.h"

void filling_struct(t_len_column *lens, struct stat buff) {
    struct passwd *userinfo = NULL;
    struct group *groupinfo;

    if (lens->len_link < mx_intlen(buff.st_nlink))
        lens->len_link = mx_intlen(buff.st_nlink);
    userinfo = getpwuid(buff.st_uid);
    if (lens->len_user < mx_strlen(userinfo->pw_name))
        lens->len_user = mx_strlen(userinfo->pw_name);
    groupinfo = getgrgid(buff.st_gid);
    if (groupinfo != NULL) {
        if (lens->len_gid < mx_strlen(groupinfo->gr_name))
            lens->len_gid = mx_strlen(groupinfo->gr_name);
    }
    else
        if (lens->len_gid < mx_intlen(userinfo->pw_gid))
            lens->len_gid = mx_intlen(userinfo->pw_gid);
    if (lens->len_size < mx_intlen(buff.st_size))
       lens->len_size = mx_intlen(buff.st_size);
}

void len_difference(t_diff_len *l, t_len_column *lens, struct stat buff) {   
    struct passwd *userinfo = NULL;
    mx_memset(l, 0, sizeof(t_diff_len));

    l->diff_link = lens->len_link;
    l->diff_size = lens->len_size;
    l->diff_user = lens->len_user;
    // int tmp1 = lens->len_link
    // int tmp2 = lens->len_size
    // int tmp3 = lens->len_user

    if (lens->len_link >= mx_intlen(buff.st_nlink))
        l->diff_link -= mx_intlen(buff.st_nlink);
    if (lens->len_size >= mx_intlen(buff.st_size))
        l->diff_size -= mx_intlen(buff.st_size);
    userinfo = getpwuid(buff.st_uid);
    if (lens->len_user >= mx_strlen(userinfo->pw_name))
        l->diff_user -= mx_strlen(userinfo->pw_name);
    // printf("diff link = %d | ", l->diff_link);
    // printf("diff size = %d | ", l->diff_size);
    // printf("diff userName = %d | ", l->diff_user);

    // return l;
}
                                                        // ________________________
void total_blocks(char *filename, t_len_column *lens) { //|<--*  кол-во блоков     |
    DIR *dp;                                            //| и заполнение структуры |
    struct dirent *dirp;                                //|________________________|
    struct stat buff;
    long long total = 0;

    mx_memset(lens, 0, sizeof(t_len_column));
    if ((dp = opendir(filename)) != NULL) {
        mx_printstr("total ");
        while ((dirp = readdir(dp)) != NULL) {
        	lstat(dirp->d_name, &buff); // ___________________________
            filling_struct(lens, buff); //|<--* запоминаю максимальную|
           	total += buff.st_blocks;    //| длину для каждого значения|
        }                               //|___________________________|
        mx_printint(total);
        mx_printchar('\n');
    }
}


void my_getuid(char *filename, int n) {
    struct passwd *userinfo = NULL;
    // struct group *groupinfo;
    uid_t userid;
    struct stat buf;

    lstat(filename, &buf);
    userid = buf.st_uid;
    // groupinfo = getgrgid(buff.st_gid);
    userinfo = getpwuid(userid);
    if (userinfo != NULL) {
        mx_printstr(userinfo->pw_name);
        mx_print_spaces(n);
        mx_print_spaces(2);
    }
}

static void mx_itoa_to_string(unsigned int number, int n) {
	char *str = mx_itoa(number);

    mx_print_spaces(n);
	mx_printstr(str);
	mx_print_spaces(1);
	free(str);
}

void my_time(char *filename) {
    struct stat *file_info = malloc(sizeof(struct stat));
    char *sub;
    time_t sec = time(NULL);
    
    lstat(filename, file_info);
    if ((sec - (file_info->st_mtime)) > 15552000 || sec < 0) {
    	sub = mx_substr(ctime(&(file_info->st_mtime)), 4, 10);
    	mx_printstr(sub);
    	free(sub);
    	mx_print_spaces(2);
    	sub = mx_substr(ctime(&(file_info->st_mtime)), 20, 24);
    	mx_printstr(sub);
    	mx_print_spaces(1);
    	free(file_info);
    }
    else {
    	sub = mx_substr(ctime(&(file_info->st_mtime)), 4, 16);
    	mx_printstr(sub);
    	mx_print_spaces(1);
    	free(file_info);
    }
    free(sub);
}

int main(int ac, char **av) {
    char pr_dost[12];
    DIR *dp;
    struct dirent *dirp;
    struct stat buff;
    t_len_column *lens = (t_len_column *)malloc(sizeof(t_len_column));
    t_diff_len *l = (t_diff_len *)malloc(sizeof(t_diff_len));
    // struct passwd *user;
    // int tmp;
    //user = getpwuid(getuid()/*buff.st_uid*/);
    if (ac != 2) {
        printf("че, ахуел сука?!");
        exit(0);
    }
    if ((dp = opendir(av[1])) == NULL) {
        printf("вафел %s\n", av[1]);
        exit(0);
    }
    total_blocks(av[1], lens);                                          // ____________________
    while ((dirp = readdir(dp)) != NULL) {                              //|<--* readdir было   |
        lstat(dirp->d_name, &buff);                                     //|  не лучшей идеей   |
        len_difference(l, lens, buff);                                  //|          :(        |
        mx_printstr(mx_mychmod(buff.st_mode, pr_dost, dirp->d_name));   //|____________________|
        mx_print_spaces(1);
        mx_itoa_to_string(buff.st_nlink, l->diff_link);
        my_getuid(dirp->d_name, l->diff_user); // эта хрень все ломает;
        mx_itoa_to_string(buff.st_gid, 0);
        mx_print_spaces(1);
        mx_itoa_to_string(buff.st_size, l->diff_size);
		my_time(dirp->d_name);
        mx_printstr(dirp->d_name);
        mx_printchar('\n');
    }
    closedir(dp);
    // printf("\ncolumn lens =\n");
    // printf("sizes %d\n", lens->len_size);
    // printf("links %d\n", lens->len_link);
    // printf("user len %d\n", lens->len_user);
    // printf("gid %d\n", lens->len_gid);
    system("leaks -q uls");
    exit(0);
}

