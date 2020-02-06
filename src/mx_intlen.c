#include "uls.h"

int mx_intlen(long long n) {
	int i = 1;

	while (n > 9) {
		n /= 10;
		i++;
	}
	return i;
}
