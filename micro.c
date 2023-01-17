#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define STDOUT__ 1
#define ERR_ARG "Error: argument\n"
#define ERR_FILE "Error: Operation file corrupted\n"
#define HEI 1000
#define WID 1000

float abs__(float n);
int strlen__(const char *s);
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

int strlen__(const char *s) {
	int i = -1;
	while (s[++i]);
	return i;
}

int err(const char *s, FILE *fp) {
	write(STDOUT__, s, strlen__(s));
	if (fp) {
		fclose(fp);
	}
	return 1;
}

int exec(const char *path) {
	char map[HEI][WID];
	int wid, hei; char bg;
	char type; float x, y, w, h; char c;

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
		tmp = fscanf(fp, "%c %f %f %f %f %c\n", &type, &x, &y, &w, &h, &c);
		if (tmp == -1) {
			break;
		}
		if (tmp != 6 || (type != 'r' && type != 'R') || w <= 0 || h <= 0) {
			return err(ERR_FILE, fp);
		}
		float b_i = y;
		float b_j = x;
		float e_i = y + h;
		float e_j = x + w;
		for (int i = 0; i < hei; ++i) {
			for (int j = 0; j < wid; ++j) {
				if (i >= b_i && i <= e_i && j >= b_j && j <= e_j) {
					if (type == 'r'
							&& abs__(i - b_i) >= 1
							&& abs__(i - e_i) >= 1
							&& abs__(j - b_j) >= 1
							&& abs__(j - e_j) >= 1) {
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
