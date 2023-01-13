#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define STDOUT__ 1
#define ERR_ARG "Error: argument\n"
#define ERR_FILE "Error: Operation file corrupted\n"
#define HEI 1000
#define WID 1000

int strlen__(const char *str);
int err(const char *s, FILE *fp);
int exec(const char *path);

int main(int ac, char **av) {
	if (ac != 2) {
		return err(ERR_ARG, NULL);
	}
	return exec(av[1]);
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
		int b_i = (y != (int) y && y > 0 ? y + 1 : y);
		int b_j = (x != (int) x && x > 0 ? x + 1 : x);
		int e_i = y + h - (y + h < 0 ? 1 : 0);
		int e_j = x + w - (x + w < 0 ? 1 : 0);
		for (int i = 0; i < hei; ++i) {
			for (int j = 0; j < wid; ++j) {
				if (i >= b_i && i <= e_i && j >= b_j && j <= e_j) {
					if (type == 'r' && i != b_i && i != e_i && j != b_j && j != e_j) {
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
