#include "cub3d.h"
#define Pi 3.14
#define MAXINT 2147483647
#define TILESIZE 64
void    cast(float ray);

int		rayFacingDown(float angle);
int		rayFacingLeft(float angle);
int rayFacingLeft(float angle);
char		**ft_split(char const *s, char c);
int         ft_atoi(char *s);

void *img;
int a,b,c;
int *data;

//Field of Vu
float fov;
int v;
//
//Mini Map
float minimap;
//columnId
int columnId;

//Number of rays and Rays_width
float   Rays_width;
unsigned int   Num_rays;
// height and Weight of the map
int height;
int width;

// mlx variables
void *mlx_ptr;
void *win_ptr;

//indices for walls
int		tab[3];

// rays
typedef struct s_text
{
	unsigned int	*txtr1;
	unsigned int	*txtr2;
	unsigned int	*txtr3;
	unsigned int 	*txtr4;
	unsigned int 	*sprite;
	int				txt_x;
	int				txt_y;
}text;

text t;
typedef struct s_castRay{
	float horizontalx;
	float horizontaly;
	float verticalx;
	float verticaly;
	float rayAngle;
	float wallHitx;
	float wallHity;
	float distance;
	float isRayFacingDown;
}castRay;

//castRay cast;

//Screen resolution
typedef struct s_Rays{
	float *rays;
	float rayHight;
	castRay cast;
}Rays;

Rays r;
Screen sc;

// stock map into lines
char **lines;

float rayAngle;
typedef struct s_sprite
{
	float x;
	float y;
}sprite;

sprite s;

typedef struct s_player{
		float x;
		float y;
		float radius;
		float turnDirection; // -1 if left, +1 if right
		float walkDirection; // -1 if back, +1 if front
		float rotationAngle; // = Math.PI / 2;
		float moveSpeed; // = 3.0;
		float rotationSpeed; // = 3 * (Math.PI / 180);
		short walk_for;
		short walk_back;
		short cam_left;
		short cam_right;
		int		look;
		short	look_up;
		short	look_down;
}player;

player p;

//Normalize Angle;
float	normalize(float rayAngle)
{
	rayAngle = fmod(rayAngle, 2 * Pi);
	while(rayAngle < 0)
		rayAngle += 2 * Pi;
	// else
	// 	rayAngle = rayAngle * (-1);
	return(rayAngle);
}

int keypress(int key)
{
	printf("%d\n", key);
	if (key == 13)
		p.walk_for = 1;
	if (key == 1)
		p.walk_back = 1;
	if (key == 123)
		p.cam_left = 1;
	if (key == 124)
		p.cam_right = 1;
	if (key == 126)
		p.look_up = 1;
	else if (key == 125)
		p.look_down = 1;
	return (0);
}

int keyreleased(int key)
{
	if (key == 13)
		p.walk_for = -1;
	if (key == 1)
		p.walk_back = -1;
	if (key == 123)
		p.cam_left = -1;
	if (key == 124)
		p.cam_right = -1;
	if (key == 126)
		p.look_up = -1;
	else if(key == 125)
		p.look_down = -1;
	return(0);
}

void    put_square(int x, int y, unsigned int color)
{
	int y_last;
	int x_last;
	int y_init;
	int savey;

	x_last = x + TILESIZE;
	y_last = y + TILESIZE;
	y_init = y;
	while(x < x_last)
	{
		y = y_init;
		while(y < y_last)
		{
				if  (color == 1)
				{
					//mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x5a5a97);
					data[x + y * sc.w] = 0xFFFFFF;
				}
				else if (color == 0)
				{
					//mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0xbbcedd);
					data[x + y * sc.w] = 0xbbcedd;
				}
			y++;
		}
		x++;
	}
}

void    drawLine(float angle, unsigned int color, int indice)
{
	float	x;
	float	y;
	float distancex;
	float distancey;
	int i;
	float deltax;
	float deltay;
	float m;
	float m2;
	float xincrement;
	float yincrement;
	int		steps;

	// clock_t start = clock();
	x = p.x;
	y = p.y;
	distancex = x + 100 * cos(angle);
	distancey = y + 100 * sin(angle);
	deltax = distancex - x;
	deltay = distancey - y;
	if (indice == 1)
	{
		deltax = r.cast.wallHitx - x;
		deltay = r.cast.wallHity - y;
	}
	m = deltay / deltax;
	if(fabs(deltax) > fabs(deltay))
		steps = fabs(deltax);
	else
		steps = fabs(deltay);
	xincrement = deltax / steps;
	yincrement = deltay / steps;
	i = 0;
	while(i < steps)
	{
		data[(int )x + (int )y * sc.w] = color;
		// if(floor(x) == floor(r.cast.wallHitx) && floor(y) == floor(r.cast.wallHity))
		// 	break;
		x += xincrement;
		y += yincrement;
		i++;
	}
// 	clock_t end = clock();
// 	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
// 	printf("ray is %f\n", time_spent);
}
void	rectangle(int e, int L, unsigned color)
{
	int i;
	int j;
	int c;
	int i_end;
	int save_x;

	save_x = e;
	i = e;
	c = 0;
	e = 0;
	j = sc.h / 2 - r.rayHight / 2 + p.look;
		c = j;
		while(e < j)
		{
			data[(int)i + (int)e *sc.w] = 0x77b5fe;
			e++;
		}
		if(j < 0)
		{
			j = 0;
		}
		while(j - c <= r.rayHight && j < sc.h)
		{
			t.txt_y = (int)((j - c) * TILESIZE) / r.rayHight;
			if(tab[0] == 0 && tab[2] == 1)
				data[(int )i + (int )j * sc.w] = t.txtr1[t.txt_x + TILESIZE * t.txt_y];
			else if(tab[0] == 0 && tab[2] == 0)
				data[(int )i + (int )j * sc.w] = t.txtr2[t.txt_x + TILESIZE * t.txt_y];
			else if (tab[0] == 1 && tab[1] == 1)
				data[(int )i + (int )j * sc.w] = t.txtr4[t.txt_x + TILESIZE * t.txt_y];
			else if (tab[0] == 1 && tab[1] == 0)
				data[(int )i + (int )j * sc.w] = t.txtr3[t.txt_x + TILESIZE * t.txt_y];
			j++;
		}
}

void	render3d(int i)
{
	float projectDistance;
	unsigned int	color;

	if(r.cast.distance < 0)
	{
		tab[0] = 0;
		r.cast.distance *= (-1);
	}
	else
	{
		tab[0] = 1;
		color = 0xFFFFFF;
	}
	if(rayFacingLeft(normalize(r.rays[i])))
		tab[1] = 1;
	else
		tab[1] = 0;
	if(rayFacingDown(normalize(r.rays[i])))
		tab[2] = 1;
	else
		tab[2] = 0;	
	if(tab[0] == 0)
		t.txt_x = (int)r.cast.wallHitx % TILESIZE;
	else
		t.txt_x = (int)r.cast.wallHity % TILESIZE;
	projectDistance = (sc.w / 2) / tan(fov / 2);
	r.cast.distance = cos(p.rotationAngle - r.rays[i]) * r.cast.distance;
	r.rayHight = (projectDistance / r.cast.distance) * TILESIZE;
	rectangle(i, 2, color);
}

void    drawRays(void)
{
	// clock_t start = clock();
	int i;
	unsigned int color;

	i = 0;
	while(i < Num_rays)
	{
		color = 0xfa2c34;
		cast(r.rays[i]);
		//drawLine(r.rays[i], color, 1);
		render3d(i);
		i+=1;
	}
}

void    render(int height, int width, char **lines, int indice)
{
	int a;
	int b;
	int x;
	int y;

	y = 0;
	a = 0;
	while(y < height)
	{
		x = 0;
		b = 0;
		while(x < width)
		{
			//printf("|%c|", lines[a][b]);
			// if(lines[a][b] == '1')
			// {
			// 	put_square(x, y, 1);
			// }
			// else
			// {
			// 	put_square(x, y, 0);
			// }
			if(lines[a][b] == 'N' && indice == 0)
			{
				//printf("loool");
				p.x = x + TILESIZE / 2;
				p.y = y + TILESIZE / 2;
				//mlx_pixel_put(mlx_ptr, win_ptr, p.x, p.y, 0x000000);
				//data[(int )p.x + (int )p.y * sc.w] = 0x000000;
				//Line();
				//drawLine(p.rotationAngle, 0x000000);
				//printf("|%f|", p.rotationAngle + fov / 2);
			}
			if(lines[a][b] == '2' && indice == 0)
			{
				s.x = x;
				s.y = y;
			}
			// else if(indice > 0)
			// {
			// 	data[(int )p.x + (int )p.y * sc.w] = 0x000000;
			// 	//Line;
			// 	drawRays();
			// 	drawLine(p.rotationAngle, 0x000000);
			// 	//drawLine();
			// }
			x+=TILESIZE;
			b++;
		}
		y+=TILESIZE;
		a++;
	}
	if(indice > 0)
	{
		data[(int )p.x + (int )p.y * sc.w] = 0x000000;
		//Line;
		drawRays();
		//drawLine(p.rotationAngle, 0x000000);
	}
}

void    printmatrice(char **tab)
{
	int a;
	int b;
	
	a = 0;
	while (tab[a])
	{
		b = 0;
		while(tab[a][b])
		{
			printf("%c|", tab[a][b]);
			b++;
		}
		printf("\n");
		a++;
	}
}
int checknextstep(int i)
{
	int a;
	int b;

	if (i == 1)
	{
		if (p.rotationAngle < 1.58 && p.rotationAngle > 1.56)
		{
			b = (int )((p.x + cos(p.rotationAngle) * p.moveSpeed) / TILESIZE);
			a = (int )((p.y + 1 + sin(p.rotationAngle) * p.moveSpeed) / TILESIZE);
		}
		else 
		{
		b = (int )((p.x + cos(p.rotationAngle) * p.moveSpeed) / TILESIZE);
		a = (int )((p.y + sin(p.rotationAngle) * p.moveSpeed) / TILESIZE);
		}
		if(lines[a][b] == '0' || lines[a][b] == 'N')
			return(1);
		else
			return (0);;
	}
	else if (i == 0)
	{
		b = (int )(p.x - cos(p.rotationAngle) * p.moveSpeed) / TILESIZE;
		a = (int )(p.y - sin(p.rotationAngle) * p.moveSpeed) / TILESIZE;
		if(lines[a][b] == '0' || lines[a][b] == 'N')
			return(1);
		else
			return (0);
	}
	return(0);
}

void    player_update(void)
{
	if (p.cam_left == 1) // left
	{
		p.rotationAngle -= p.rotationSpeed;
	}
	if (p.cam_right == 1) //right
	{
		p.rotationAngle += p.rotationSpeed;
	}
	if (p.walk_for == 1)
	{
		if (checknextstep(1))
		{
			p.x += cos(p.rotationAngle) * p.moveSpeed;
			p.y += sin(p.rotationAngle) * p.moveSpeed;
		}
	}
	if (p.walk_back == 1 && checknextstep(0))
	{
		p.x -= cos(p.rotationAngle) * p.moveSpeed;
		p.y -= sin(p.rotationAngle) * p.moveSpeed;
	}
	if (p.look_up == 1)
	{
		p.look+=10;
	}
	if (p.look_down == 1)
	{
		p.look-=10;
	}
}

int		rayFacingDown(float angle)
{
	// if (angle > 0 && angle <= Pi)
	// 	return(1);
	if (sin(angle) > 0)
		return(1);
	 return(0);
}

int		rayFacingLeft(float angle)
{
	// if (angle > (Pi / 2) && angle < (3 * Pi) / 2)
	// 	return(1);
	if (cos(angle) < 0)
		return (1);
	return(0);
}

void	horizontalintersect(float rayAngle)
{
	float ystep;
	float xstep;
	float ay;
	float ax;
	float angle;

	angle = normalize(rayAngle);
	ystep = TILESIZE * (rayFacingDown(angle) ? 1 : -1);
	xstep = tan(angle) ? ystep / tan(angle) : 0;
	ay = (floor(p.y / TILESIZE) * TILESIZE) + (rayFacingDown(angle) ? TILESIZE : 0);
	ax = p.x + ((ay - p.y) / tan(angle));
	r.cast.distance = 0;
	if(!rayFacingDown(angle))
		ay--;
	while(ay >= 0 && ax >= 0 && ax < width && ay < height)
	{
		if(lines[(int )(ay) / TILESIZE][(int )(ax) / TILESIZE] == '1')
		{
			r.cast.horizontalx = ax;
			r.cast.horizontaly = ay + (!rayFacingDown(angle) ? 1 : 0);
			break;
		}
		ax += xstep;
		ay += ystep;
	}
}

void 	verticalintersect(float rayAngle)
{
	float xstep;
	float ystep;
	float ay;
	float ax;
	float angle;

	angle = normalize(rayAngle);
	xstep = TILESIZE * (rayFacingLeft(angle) ? -1 : 1);
	ystep = xstep * (tan(angle));
	ax = (floor(p.x / TILESIZE) * TILESIZE) + (!rayFacingLeft(angle) ? TILESIZE : 0);
	ay = p.y - (tan(angle) * (p.x - ax));
	if(rayFacingLeft(angle))
		ax--;
	while(ay >= 0 && ax >= 0 && ax < width && ay < height)
	{
		if(lines[(int )(ay) / TILESIZE][(int )(ax) / TILESIZE] == '1')
		{
			r.cast.verticalx = ax + (rayFacingLeft(angle) ? 1 : 0);
			r.cast.verticaly = ay;
			break;
		}
		ax += xstep;
		ay += ystep;
	}
}
void	chosePoints()
{
	float a;
	float b;

	a = sqrtf(powf(p.x - r.cast.verticalx, 2) + powf(p.y - r.cast.verticaly, 2));
	b = sqrtf(powf(p.x - r.cast.horizontalx, 2) + powf(p.y - r.cast.horizontaly, 2));
	if (a <= b)
	{
		r.cast.wallHitx = r.cast.verticalx;
		r.cast.wallHity = r.cast.verticaly;
		r.cast.distance = a;
	}
	else
	{
		r.cast.wallHitx = r.cast.horizontalx;
		r.cast.wallHity = r.cast.horizontaly;
		r.cast.distance = b * (-1);
	}
}
void	cast(float rayAngle)
{
	r.cast.wallHitx = MAXINT;
	r.cast.wallHity = MAXINT;
	r.cast.horizontalx = MAXINT;
	r.cast.horizontaly = MAXINT;
	r.cast.verticalx = MAXINT;
	r.cast.verticaly = MAXINT;
	horizontalintersect(rayAngle);
	verticalintersect(rayAngle);
	chosePoints();
}
void    castAllRays(void)
{
	int i;

	columnId = 0;
	i = 0;
	rayAngle = p.rotationAngle - (fov / 2);
	while(i < Num_rays)// && rayAngle < p.rotationAngle + (fov / 2))
	{
		r.rays[columnId] = rayAngle;
		//cast(r.rays[columnId]);
		rayAngle += fov / Num_rays;
		//("|%f|", rayAngle);
		i+=1;
		columnId++;
	}
}

void	blackscreen(void)
{
	int x;
	int y;

	x = 0;
	y = 0;
	while(x < sc.w)
	{
		y = 0;
		while(y < sc.h)
		{
			data[(int )x + (int )y * sc.w] = 0x000000;
			y++;
		}
		x++;
	}
}
int update()
{
	static char i;
	//clock_t b, e;
	mlx_hook(win_ptr , 2 , 0 ,  keypress, 0);
	mlx_hook(win_ptr, 3 , 0 ,  keyreleased,  0);
	player_update();
	castAllRays();
	mlx_clear_window(mlx_ptr, win_ptr);
	blackscreen();
	render(height, width, lines, 1);
	rendersprite();
	if (!i || i++ == 127)
		mlx_put_image_to_window(mlx_ptr, win_ptr, img, 0 , 0);
	//mlx_destroy_image(mlx_ptr, img);
	return (0);
}

int		ft_xpm(unsigned int **info, char	*file)
{
	int		fd;
	void	*img;
	int		tab[5];

	if ((fd = open(file, O_RDONLY)) < 0)
	{
		printf("Error\ninvalide path");
		return(0);
	}
	close(fd);
	img = mlx_xpm_file_to_image(mlx_ptr, file, &tab[0], &tab[1]);
	*info = (unsigned int *)mlx_get_data_addr(img, &tab[2], &tab[3], &tab[4]);
	free(img);
	return(1);
}

void	render_3dsprite(float x_offsset, float j, float spriteHeight)
{
	float save_j;
	float y_offset;

	save_j = j;	
	while((j - save_j) < spriteHeight && j < sc.h)
	{
		y_offset= (int)((j - save_j) * TILESIZE) / spriteHeight;
		data[(int )s.x + (int )j * sc.w] = t.txtr4[(int)x_offsset + (int)(TILESIZE * y_offset)];
		j++;
	}
}

void	rendersprite(void)
{
	int i;
	float distx;
	float disty;
	float dist;
	float angle[Num_rays];
	float omega;
	int intersect_y;
	int intersect_x;
	int sp_height;
	int sp_width;

	i = 0;
	//printf("angle is |%d|\n", angle[i]);
	while(i < Num_rays)
	{
		distx = s.x - p.x;
		disty = s.y - p.y;
		dist = sqrtf(powf(distx, 2) + powf(disty, 2));
		//printf("distx %f disty %f \n", distx, disty);
		//printf("%d|%d\n", s.x, s.y);
		angle[i] = atan2(disty, distx);
		angle[i] = normalize(angle[i]);
		angle[i] = (angle[i]);
		//angle[i] = r.rays[i] - (r.rays[i] - p.rotationAngle) - angle[i];
		angle[i] = r.rays[i] - angle[i]; 
		angle[i] = normalize(angle[i]);
		angle[i] = angle[i] * sc.w / fov;
		if(sc.h > sc.w)
			sp_height = (sc.h / (dist * TILESIZE));
		else
			sp_width = (sc.w / (dist * TILESIZE));
		intersect_y = (sc.h / 2) - (sp_height / 2);
		intersect_x = ((angle[i] * Pi / 180) / (fov * (Pi / 120)) * sc.w + (sc.w / 2 - sp_height / 2));
		render_3dsprite(intersect_x, intersect_y, sp_height);
		printf("angle is |%f|\n", angle[i] * 180 / Pi);
		i++;
	}
}
int		ft_textures(void)
{
	char			*file5;
	file5 = "utils/pictures/barrel.xpm";
	if(!ft_xpm(&t.txtr1, eapath))
		return(0);
	if(!ft_xpm(&t.txtr2, sopath))
		return(0);
	if(!ft_xpm(&t.txtr3, nopath))
		return(0);
	if(!ft_xpm(&t.txtr4, wepath))
		return(0);
	if(!ft_xpm(&t.sprite, file5))
		return(0);
	return(1);
}


int main()
{
	int fd;
	int a;
	int b;

	fd = open("map.txt", O_RDWR);
	if(!readfile(fd))
		return (0);
	
	a = 0;
	b = 0;
	while(lines[a])
		a++;
	height = TILESIZE * a;
	while(lines[0][b])
		b++;
	width = TILESIZE * b;

	mlx_ptr = mlx_init();
	if((!ft_textures()))
		return (0);
	win_ptr = mlx_new_window(mlx_ptr, sc.h, sc.w, "mlx 42");
	//init field of Vu
	fov = 60 * (Pi / 180);
	//init number of rays
	Rays_width = 1;
	Num_rays = sc.w / Rays_width;
	r.rays = malloc(sizeof(float) * Num_rays);
	//init params of player
	p.x = 0;
	p.y = 0;
	p.radius = 3;
	p.turnDirection = 0;
	p.walkDirection = 0;
	p.rotationAngle = Pi / 2;
	p.moveSpeed = 4;
	p.rotationSpeed = 1.5 * Pi / 180;
	p.look = 0;
	img = mlx_new_image(mlx_ptr, sc.w, sc.h);
	data = (int*)mlx_get_data_addr(img, &a, &b, &c);
	render(height, width, lines, 0);
	rendersprite();
//	mlx_hook(win_ptr , 2 , 0 ,  keypress, 0);
//	mlx_hook(win_ptr, 3 , 0 ,  keyreleased,  0);
//	mlx_key_hook()
	mlx_loop_hook(mlx_ptr, update, 0);
 	mlx_loop(mlx_ptr);
	return (0);
}