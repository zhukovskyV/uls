#include "uls.h"

static void mx_itoa_to_string(unsigned int number, int n);

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
        else if (lens->len_gid < mx_intlen(userinfo->pw_gid))
                lens->len_gid = mx_intlen(userinfo->pw_gid);
    }

    if (lens->len_size < mx_intlen(buff.st_size))
       lens->len_size = mx_intlen(buff.st_size);
}

void len_difference(t_diff_len *l, t_len_column *lens, struct stat buff) {   
    struct passwd *userinfo = NULL;
    struct group *groupinfo = NULL;
    
    l->diff_link = lens->len_link;
    l->diff_size = lens->len_size;
    l->diff_user = lens->len_user;
    l->diff_gid = lens->len_gid;
    if (lens->len_link >= mx_intlen(buff.st_nlink))
        l->diff_link -= mx_intlen(buff.st_nlink);
    if (lens->len_size >= mx_intlen(buff.st_size))
        l->diff_size -= mx_intlen(buff.st_size);
    userinfo = getpwuid(buff.st_uid);
    if (lens->len_user >= mx_strlen(userinfo->pw_name))
        l->diff_user -= mx_strlen(userinfo->pw_name);
    groupinfo = getgrgid(buff.st_gid);
    if (groupinfo != NULL) {
        if (lens->len_gid >= mx_strlen(groupinfo->gr_name))
            l->diff_gid -= mx_strlen(groupinfo->gr_name);
        else if (lens->len_gid >= mx_intlen(userinfo->pw_gid))
            l->diff_gid -= mx_intlen(userinfo->pw_gid);
    }
}

char **mx_find_path1(char **dirs_in, char *dir_name, int dir_count) {
    char **path = (char **)malloc(sizeof(char *) * (dir_count + 1));
    char *tmp = NULL;
    if (dir_name == NULL)
        return dirs_in;
    for (int i = 0; i <= dir_count; i++) {
        if (mx_strcmp(dir_name, "/") == 0)
            tmp = mx_strdup(dir_name);
        else
            tmp = mx_strjoin(dir_name, "/");
        path[i] = mx_strjoin(tmp, dirs_in[i]);
        // printf(" >>>> %s\n", path[i]);
        // printf(" >>>>%d\n", dir_count);
        if (tmp != NULL)
           free(tmp);
    }
    path[dir_count] = NULL;
    return path;
}

void total_blocks(char **files_in_dir, t_len_column *lens, char *dir_name, int count) {
    struct stat buff;
    long long total = 0;
    char **path = NULL;

    mx_memset(lens, 0, sizeof(t_len_column));
    if (dir_name != NULL)
        path = mx_find_path1(files_in_dir, dir_name, count);

        // printf("%s <<<<<<<<<<<<<<<<<\n\n", path[0]);
    for (int i = 0; path[i]; i++) {
        // printf("%s\n", path[i]);
        if (dir_name != NULL)
            lstat(path[i], &buff);
        else
            lstat(files_in_dir[i], &buff); 
        filling_struct(lens, buff);
       	total += buff.st_blocks;

    } 
    mx_printstr("total ");
    mx_printint(total);
    mx_printchar('\n');
}

void my_getgrgid(char *filename, int n) {
    struct group *groupinfo;
    struct stat buff;

    lstat(filename, &buff);
    groupinfo = getgrgid(buff.st_gid);
    if (groupinfo != NULL) {
        mx_printstr(groupinfo->gr_name);
        mx_print_spaces(n);
        mx_print_spaces(1);
    }
    else
        mx_itoa_to_string(buff.st_gid, n);
}

void my_getuid(char *filename, int n) {
    struct passwd *userinfo = NULL;
    uid_t userid;
    struct stat buff;

    lstat(filename, &buff);
    userid = buff.st_uid;
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


void mx_flag_l(char **files_in_dir, int count, char *dir_name) {
    char pr_dost[12];
    char ayaya[1024]; // <3 twitch
    struct stat buff;
    t_len_column *lens = (t_len_column *)malloc(sizeof(t_len_column));
    t_diff_len *l = (t_diff_len *)malloc(sizeof(t_diff_len));
    char **path = NULL;

    if (dir_name != NULL)
        path = mx_find_path1(files_in_dir, dir_name, count);
    else
        path = mx_find_path1(files_in_dir, ".", 1);
    total_blocks(files_in_dir, lens, dir_name, count);
    for (int i = 0; path[i]; i++) {
        lstat(path[i], &buff);
        len_difference(l, lens, buff);
        mx_printstr(mx_mychmod(buff.st_mode, pr_dost, path[i]));
        mx_print_spaces(1);
        mx_itoa_to_string(buff.st_nlink, l->diff_link);
        my_getuid(path[i], l->diff_user);
        my_getgrgid(path[i], l->diff_gid);
        mx_itoa_to_string(buff.st_size, l->diff_size + 1);
        my_time(path[i]);
        mx_printstr(files_in_dir[i]);
        if (pr_dost[0] == 'l')
            readlink(path[i], ayaya, 1024);
        if (ayaya[0] != '\0') {
            mx_printstr(" -> ");
            mx_printstr(ayaya);
        }
        mx_printchar('\n');
        mx_bzero(ayaya, sizeof(ayaya));
        // ayaya[0] = '\0';
        // bzero(lin, sizeof(lin));
    }
        // printf("size link %d\n", lens->len_link);
        // printf("size size %d\n", lens->len_size);
        // printf("size user %d\n", lens->len_user);
        // printf("size gid  %d || ",lens->len_gid);
        // printf("diff gid  %d\n",l->diff_gid);
    // system("leaks uls");
}


