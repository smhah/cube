#include "mlx.h"
#include "utils/get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

typedef struct s_Screen{
	int	h;
	int w;
}Screen;

char    *nopath;
char    *sopath;
char    *wepath;
char    *eapath;

size_t	ft_strlen(const char *str);
int		rayFacingDown(float angle);
int		rayFacingLeft(float angle);
int rayFacingLeft(float angle);
char		**ft_split(char const *s, char c);
int         ft_atoi(char *s);
int 	readfile(int fd);
char    **ft_cube3d(int fd);
void    printmatrice(char **tab);
void	rendersprite(void);