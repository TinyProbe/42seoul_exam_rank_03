#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define STDOUT__ 1
#define ERR_ARG "Error: argument\n"
#define ERR_FILE "Error: Operation file corrupted\n"
#define HEI 1000
#define WID 1000

float abs__(float n);
int strlen__(const char *str);
int err(const char *s, FILE *fp);
int exec(const char *path);

int main(int ac, char **av) {
	if (ac != 2) {
		return err(ERR_ARG, NULL);
	}
	return exec(av[1]);
}

float abs__(float n) {
	if (n < 0) {
		return -n;
	}
	return n;
}

int strlen__(const char *str) {
	int i = -1;
	while (str[++i]);
	return i;
}

int err(const char *s, FILE *fp) {
	write(STDOUT__, s, strlen__(s));
	if (fp)
		fclose(fp);
	return 1;
}

int exec(const char *path) {
	char map[HEI][WID];
	int wid, hei; char bg;
	char type; float x, y, r; char c;

	FILE *fp = fopen(path, "r");
	if (!fp) {
		return err(ERR_FILE, fp);
	}
	int tmp = fscanf(fp, "%d %d %c\n", &wid, &hei, &bg);
	if (tmp != 3 || wid < 1 || wid > 300 || hei < 1 || hei > 300) {
		return err(ERR_FILE, fp);
	}
	for (int i = 0; i < hei; ++i) {
		memset(map[i], bg, wid);
	}
	while (1) {
		tmp = fscanf(fp, "%c %f %f %f %c\n", &type, &x, &y, &r, &c);
		if (tmp == -1) {
			break;
		}
		if (tmp != 5 || (type != 'c' && type != 'C') || r <= 0) {
			return err(ERR_FILE, fp);
		}
		for (int i = 0; i < hei; ++i) {
			for (int j = 0; j < wid; ++j) {
				float xx = abs__(x - j) * abs__(x - j);
				float yy = abs__(y - i) * abs__(y - i);
				float dist = sqrtf(xx + yy);
				if (dist <= r) {
					if (type == 'c' && abs__(dist - r) >= 1) {
						continue;
					}
					map[i][j] = c;
				}
			}
		}
	}
	for (int i = 0; i < hei; ++i) {
		write(STDOUT__, map[i], wid);
		write(STDOUT__, "\n", 1);
	}
	fclose(fp);
	return 0;
}
