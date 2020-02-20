#include "uls.h"

char *whole_part(int a, long double f) {
	char *tmp;
	char *str;

	f = (f < 0) ? -f : f;
	tmp = mx_itoa(f);
	if (!(str = (char *)malloc(sizeof(char) * (a + 1000))))
		return (NULL);
	str = mx_strcat(str, tmp);
	free(tmp);
	str = mx_strcat(str, ".");
	return (str);
}

char *round_f(char *str) {
	int	l;
	char *tmp;

	l = mx_strlen(str) - 1;
	if (str[l] > '4')
		l -= (str[l - 1] == '.') ? 2 : 1;
	str[l]++;
	while (l > -1 && str[l] > '9') {
		str[l] = '0';
		if (str[l - 1] == '.')
			l--;
		str[l - 1]++;
		l--;
	}
	if (l == -1) {
		tmp = str;
		str = mx_strjoin("1", str);
		free(tmp);
	}
	return (str);
}

// int	l_case(char *str) {
// 	if (mx_strcmp(str, MIN_STR) == 0) {
// 		free(str);
// 		return (1);
// 	}
// 	return (0);
// }

char *mx_ftoa(long double f) {
	char *str;
	char *tmp;
	size_t n;
	int a = 0;
	int i = 0;
	long double zz = f;

	f < 0 ? f = f * -1 : false;
	// a = p->pr;
	// p->dot ? a++ : (a = 7);
	while (zz > 1024) {
            zz /= 1024.0;
            i++;
        }
            // printf(">>>%s\n", zz);
	a = i;
	str = whole_part(a, f);
	// if (l_case(str) == 1)
	// 	return (mx_strdup("0.0000000"));
	f = f - (size_t)f;
	n = 0;
	while (a--) {
		f -= n;
		n = f *= 10;
		tmp = mx_itoa(n);
		str = mx_strcat(str, tmp);
		free(tmp);
	}
	str = round_f(str);
	return (str);
}
