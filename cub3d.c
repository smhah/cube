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

int test;
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
int		g_i;
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
	int id;
	castRay cast;
}Rays;

typedef struct s_row{
	int posax[1000];
    int posay[1000];
	float xhit[1000];
	float yhit[1000];
	float dist[1000];
    int id;
	float rayHight;
	//int id;
	castRay cast;
}row;

row rows;

int vorh[10000];

Rays r;
Screen sc;

// stock map into lines
char **lines;

float rayAngle;
typedef struct s_sprite
{
	float	x[10000][1000];
	float	y[10000][1000];
	float	xh[10000];
	float	xv[10000][1000];
	float	yv[10000][1000];
	float	yh[10000];
	float	dist[10000][1000];
	float	xc[10000];
	float	yc[10000];
	float	xprime[10000][1000];
	float	yprime[10000][1000];
	float	delta[10000];
	float	xofset[10000][1000];
	int		info[10000];
	int		sprite;
	int		saveh;
	int		savev;
    int     saveax[1000];
    int     saveay[1000];
    int     saves[1000][1000];
    int      i;
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

void DDA(int X0, int Y0, int X1, int Y1) 
{ 
    // calculate dx & dy 
    int dx = X1 - X0; 
    int dy = Y1 - Y0; 
	int i;

	i = 0;
    // calculate steps required for generating pixels 
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy); 
  
    // calculate increment in x & y for each steps 
    float Xinc = dx / (float) steps; 
    float Yinc = dy / (float) steps; 
  
    // Put pixel for each step 
    float X = X0; 
    float Y = Y0; 
    while(i <= steps) 
    { 
		data[(int)(X) + (int)Y * sc.w] = 0xbbcc21;// put pixel at (X,Y) 
        X += Xinc;           // increment in x at each step 
        Y += Yinc;           // increment in y at each step 
        //delay(100);          // for visualization of line- 
		i++;                      // generation step by step 
    } 
}

int keypress(int key)
{
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
				else if (color == 2)
					data[x + y * sc.w] = 0x000000;
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
		while(j < sc.h)
		{
			data[(int)i + (int)j *sc.w] = 0xFFFFFF;
			j++;
		}
}

float distance(int x1, int y1, int x2, int y2) 
{ 
    // Calculating distance 
    return sqrt(pow(x2 - x1, 2) +  
                pow(y2 - y1, 2) * 1.0); 
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

void	rectosprite(float spriteHeight, int i)
{
	int j;
	int y;

	j = sc.h / 2 - spriteHeight / 2 + p.look;
	c = j;
	if(j < 0)
		j = 0;
	while(j - c < spriteHeight && j < sc.h)
	{
		y = (int)((j - c) * TILESIZE) / spriteHeight;
			if(t.sprite[(int)s.xofset[r.id][g_i] + TILESIZE * y] != 0xff000000 && t.sprite[(int)s.xofset[r.id][g_i] + TILESIZE * y] != 0x000000)
				data[(int )i + (int )j * sc.w] = t.sprite[(int)s.xofset[r.id][g_i] + TILESIZE * y];
				//printf("color is%x\n", data[(int )i + (int )j * sc.w] = t.sprite[(int)s.xofset[r.id][g_i] + TILESIZE * y]);
		j++;
		//printf("heho\n");
	}
}

void	render3dsprite()
{
	float projectDistance;
	float spriteHeight;
	float dist;

	dist = sqrtf(powf(p.x - s.xc[r.id], 2) + powf(p.y - s.yc[r.id], 2));
	projectDistance = (sc.w / 2) / tan(fov / 2);
	//s.dist[r.id][g_i] = sqrtf(powf(p.x - s.xc[r.id], 2) + powf(p.y - s.yc[r.id], 2));//distance(p.x, s.xc[r.id], p.y, s.yc[r.id]);
	//s.dist[r.id][g_i] = cos(p.rotationAngle - r.rays[r.id]) * r.cast.distance;
	//printf("%f|%f\n", s.xc[r.id], s.yc[r.id]);
	spriteHeight = (projectDistance / dist) * TILESIZE;
	//printf("%f|%f\n", s.xprime[r.id][g_i], s.yprime[r.id][g_i]);
	rectosprite(spriteHeight, r.id);
}
void    drawRays(void)
{
	// clock_t start = clock();
	unsigned int color;
	int i;

	r.id = 0;
	while(r.id < Num_rays)
	{
		s.sprite = 0;
		color = 0xfa2c34;
		cast(r.rays[r.id]);
		// if(r.id ==1 / 2)
		// 	drawLine(r.rays[r.id], 0xFFFFFF, 1);
		// else
		//	drawLine(r.rays[r.id], color, 1);
		 render3d(r.id);
		g_i = s.sprite;
		while(g_i--)
		{
			if(rows.xhit[g_i] < MAXINT - 100)
			{
				rendersprite();
				 if(s.xofset[r.id][g_i] > 0 && s.xofset[r.id][g_i] < 70)
				 	render3dsprite();
			}
		}
			//printf("s.x[r.id][g_i] = %f\n", s.x[r.id][g_i]);
		r.id+=1;
		//printf("o\n");
	}
}

void	rendersprite(void)
{
	float y2;
	float x2;
	float a;
	float b;
	float angle;
	float c;
	float x;
	float y;
	float dist;

	angle = normalize(r.rays[r.id]);
	angle = (Pi - angle) * (-1);
	if(rayFacingLeft(normalize(r.rays[r.id])))
		tab[0] = 1;
	else
		tab[0] = 0;
	if(rayFacingDown(normalize(r.rays[r.id])))
		tab[1] = 1;
	else
		tab[1] = 0;
	//printf("|%f|%f|\n", s.x[r.id][g_i], s.y[r.id][g_i]);
	if(rows.xhit[g_i] < 0)
	{
		rows.xhit[g_i] *= -1;
		s.xc[r.id] = floor((rows.xhit[g_i]) / TILESIZE) * TILESIZE + ((tab[0] == 1) ? TILESIZE / (-2) : (TILESIZE / 2));
		s.yc[r.id] = floor(rows.yhit[g_i] / TILESIZE) * TILESIZE + TILESIZE / 2;
		tab[2] = 1;
	}
	else
	{
	s.xc[r.id] = floor(rows.xhit[g_i] / TILESIZE) * TILESIZE +  TILESIZE / 2;//(rayFacingLeft(normalize(r.rays[r.id])) ? TILESIZE / 2 : (TILESIZE + TILESIZE / 2));
	s.yc[r.id] = floor(rows.yhit[g_i] / TILESIZE) * TILESIZE + ((tab[1] == 1) ? TILESIZE / 2 : TILESIZE / (-2));
	tab[2] = 0;
	}
	//printf("|%f|%f|\n", s.xc[r.id], s.yc[r.id]);
	 //(rayFacingDown(normalize(r.rays[r.id])) ? TILESIZE / 2 : TILESIZE / (-2));
	b = atan2(p.y - s.yc[r.id], p.x - s.xc[r.id]);
	b = normalize(b);
	a = angle - b;
	//a = angle;
	//printf("|b is%f|, |a is%f|\n", b * 180 / Pi, a * 180 / Pi);
	//a = Pi - a;
	//a = a + Pi / 2;
	s.xprime[r.id][g_i] = p.x + (s.xc[r.id] - p.x) * cos(a) - (s.yc[r.id] - p.y) * sin(a);
	s.yprime[r.id][g_i] = p.y + (s.xc[r.id] - p.x) * sin(a) + (s.yc[r.id] - p.y) * cos(a);
	//data[(int )s.xprime[r.id][g_i] + (int )s.yprime[r.id][g_i] * sc.w] = 0xFFFFFF;
	b = Pi - b;
	x = (s.xc[r.id] - (TILESIZE / 2) * cos(b));
	y = (s.yc[r.id] + (TILESIZE / 2) * sin(b));
	//s.xofset[r.id][g_i] = sqrtf(powf(s.xprime[r.id][g_i] - x, 2) + powf(s.yprime[r.id][g_i] - y, 2));
	s.xofset[r.id][g_i] = sqrtf(powf(s.xprime[r.id][g_i] - s.xc[r.id], 2) + powf(s.yprime[r.id][g_i] - s.yc[r.id], 2));
	//printf("|%f|\n", s.xofset[r.id][g_i]);
	DDA(s.xprime[r.id][g_i], s.yprime[r.id][g_i], s.xc[r.id], s.yc[r.id]); 
	if(tab[1] == 0)
	{
		if(tab[2] == 0)
		{
			if(s.xprime[r.id][g_i] >= s.xc[r.id])
				s.xofset[r.id][g_i] += TILESIZE / 2;
			else
				s.xofset[r.id][g_i] = TILESIZE - TILESIZE / 2 - s.xofset[r.id][g_i];
		}
		else
		{
			if(tab[0] == 1)
			{
				if(s.yprime[r.id][g_i] <= s.yc[r.id])
					s.xofset[r.id][g_i] += TILESIZE / 2;
				else
					s.xofset[r.id][g_i] = TILESIZE - TILESIZE / 2 - s.xofset[r.id][g_i];
			}
			else
			{
				if(s.yprime[r.id][g_i] >= s.yc[r.id])
					s.xofset[r.id][g_i] += TILESIZE / 2;
				else
					s.xofset[r.id][g_i] = TILESIZE - TILESIZE / 2 - s.xofset[r.id][g_i];
			}
		}
	}
	else
	{
		if(tab[2] == 0)
		{
			if(s.xprime[r.id][g_i] >= s.xc[r.id])
				s.xofset[r.id][g_i] = TILESIZE - TILESIZE / 2 - s.xofset[r.id][g_i];
			else
				s.xofset[r.id][g_i] += TILESIZE / 2;
		}
		else
		{
			if(tab[0] == 1)
			{
				if(s.yprime[r.id][g_i] >= s.yc[r.id])
					s.xofset[r.id][g_i] = TILESIZE - TILESIZE / 2 - s.xofset[r.id][g_i];
				else
					s.xofset[r.id][g_i] += TILESIZE / 2;
			}
			else
			{
				if(s.yprime[r.id][g_i] <= s.yc[r.id])
					s.xofset[r.id][g_i] = TILESIZE - TILESIZE / 2 - s.xofset[r.id][g_i];
				else
					s.xofset[r.id][g_i] += TILESIZE / 2;
			}
		}
	}
	// a = angle;
	// y2 = (s.xc[r.id] - b * s.yc[r.id] + a * p.y - p.x) / (a - b);
	// x2 = b * y2 - b * s.yc[r.id] + s.xc[r.id];
	//printf("centeris : %f|%f\n", y2, x2);
	// data[(int )x2 + (int )y2 * sc.w] = 0x00bbaa;
	// data[(int )s.xc[r.id] + (int )s.yc[r.id] * sc.w] = 0xFFFFFF;
	// data[(int )rows.xhit[i] + (int )s.y[r.id][g_i] * sc.w] = 0xFFFFFF;
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
    s.i = 0;
	while(ay >= 0 && ax >= 0 && ax < width && ay < height)
	{
		if(lines[(int )(ay) / TILESIZE][(int )(ax) / TILESIZE] == '2')
		{
			rows.posax[rows.id] = floor(ax  / TILESIZE);
			//printf("/%d/\n", rows.posax[rows.id]);
			rows.posay[rows.id] = floor(ay / TILESIZE);
			rows.xhit[rows.id] = ax;
			rows.yhit[rows.id++] = ay + (!rayFacingDown(angle) ? 1 : 0);
			rows.posax[rows.id] = MAXINT;
			rows.posay[rows.id] = MAXINT;
			rows.xhit[rows.id] = MAXINT;
			rows.yhit[rows.id] = MAXINT;
			// s.xh[s.sprite] = ax;
			// s.yh[s.sprite] = ay + (!rayFacingDown(angle) ? 1 : 0);
			// s.sprite++;
			// s.xh[s.sprite] = MAXINT;
			// s.yh[s.sprite] = MAXINT;
			//printf("findhor\n");
		}
		if(lines[(int )(ay) / TILESIZE][(int )(ax) / TILESIZE] == '1')
		{
			r.cast.horizontalx = ax;
			r.cast.horizontaly = ay + (!rayFacingDown(angle) ? 1 : 0);
			// if(check == 0)
			// {
			// 	rows.xhit[rows.id] = MAXINT;
			// 	rows.yhit[rows.id] = MAXINT;
			// }
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
    s.i = 0;
	while(ay >= 0 && ax >= 0 && ax < width && ay < height)
	{
		if(lines[(int )(ay) / TILESIZE][(int )(ax) / TILESIZE] == '2')
		{
			rows.posay[rows.id] = floor(ay / TILESIZE);
			rows.posax[rows.id] = floor((ax)  / TILESIZE);
			rows.xhit[rows.id] = (ax + (rayFacingLeft(angle) ? 1 : 0)) * (-1);
			rows.yhit[rows.id++] = ay;
			rows.posax[rows.id] = MAXINT;
			rows.posay[rows.id] = MAXINT;
			rows.xhit[rows.id] = MAXINT;
			rows.yhit[rows.id] = MAXINT;
			// s.xv[r.id][s.sprite] = ax + (rayFacingLeft(angle) ? 1 : 0);
			// s.yv[r.id][s.sprite] = ay;
			// s.sprite++;
			// s.xv[r.id][s.sprite] = MAXINT;
			// s.yv[r.id][s.sprite] = MAXINT;
            //printf("|{%d}|\n", s.sprite);
			//printf("findver\n");
		}
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

int		findinfo(int posx,int posy)
{
	int i;

	i = 0;
	while(i < 10)
	{
		if(rows.posax[i] == posx && rows.posay[i] == posy)
			return(i);
			i++;
	}
	return(i);
}


void	compare(int pos1x, int pos1y, int pos2x, int pos2y)
{
	//printf("miaw\n");
	if(!rayFacingLeft(normalize(p.rotationAngle)))
	{
		if(pos1x < pos2x)
		{
			s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
			s.yh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
		}
		else if(pos1x > pos2x)
		{
			s.xh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
			s.yh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
		}
		else
		{
			if(rayFacingDown(normalize(p.rotationAngle)))
			{
				if(pos1y < pos2y)
				{
					s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
					s.yh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
				}
				else
				{
					s.xh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
					s.yh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
				}
			}
			else
			{
				if(pos1y < pos2y)
				{
					s.xh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
					s.yh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
				}
				else
				{
					s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
					s.yh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
				}
			}
			
		}
		
	}
	else
	{
		//printf("goood\n");
		//printf("%d|%d\n", pos1x, pos2x);
		if(pos1x < pos2x)
		{
			s.xh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
			s.yh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
			//printf("goood\n");
		}
		else if(pos1x > pos2x)
		{
			s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
			s.yh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
			//printf("goood\n");
		}
		else
		{
			if(rayFacingDown(normalize(p.rotationAngle)))
			{
				if(pos1y < pos2y)
				{
					s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
					s.yh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
				}
				else
				{
					s.xh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
					s.yh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
				}
			}
			else
			{
				if(pos1y < pos2y)
				{
					s.xh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
					s.yh[rows.id] = rows.xhit[findinfo(pos2x, pos2y)];
				}
				else
				{
					s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
					s.yh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
				}
			}
			
		}
	}
}

void	sort(void)
{
	int i;
	int j;
	float tmp;
	float e;

	i = 0;
	j = 1;
	while(rows.xhit[i] < MAXINT - 100)
	{
		e = rows.xhit[i];
		if( e < 0)
			e = e * (-1);
		rows.dist[i] = sqrtf(powf(p.x - e, 2) + powf(p.y - rows.yhit[i], 2));
		i++;
	}
	rows.dist[i] = MAXINT;
	i = 0;
	while(rows.dist[i] < MAXINT - 100)
	{
		j = i + 1;
		while(rows.dist[j] < MAXINT - 100)
		{
			if(rows.dist[i] > rows.dist[j])
			{
				tmp = rows.dist[i];
				rows.dist[i] = rows.dist[j];
				rows.dist[j] = tmp;
				tmp = rows.xhit[i];
				rows.xhit[i] = rows.xhit[j];
				rows.xhit[j] = tmp;
				tmp = rows.yhit[i];
				rows.yhit[i] = rows.yhit[j];
				rows.yhit[j] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	// printf("----------\n");

	// 	printf("1 is %f, 2 is %f\n", rows.xhit[i], rows.xhit[i + 1]);
	// 	i++;
	// printf("----------\n");
}
void	class(void)
{
	int pos1x;
	int pos1y;
	int pos2x;
	int pos2y;
	int i;

	i = 0;
	// pos1x = rows.posax[i];
	// pos1y = rows.posay[i++];
	// printf("/%d/\n", findinfo(pos1x, pos1y));
	// s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
	// s.yh[rows.id] = rows.yhit[findinfo(pos1x, pos1y)];
	//rows.id++;
	// printf("{%f|%f}\n", s.xh[rows.id], s.yh[rows.id]);
	// printf("[%f|%f]\n", rows.xhit[0], rows.yhit[0]);

	//printf("/%f|%f/\n", rows.xhit[0], rows.yhit[0]);
	//printf("/%f|%f/%d>\n", rows.xhit[1], rows.yhit[1], s.sprite);

	while(rows.xhit[i] < MAXINT - 100)
	{
		if(i == 0)
		{
			pos1x = rows.posax[i];
			pos1y = rows.posay[i++];
			s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
			s.yh[rows.id] = rows.yhit[findinfo(pos1x, pos1y)];
			rows.id++;
		}
		if(i > 1)
		{
			pos1x = rows.posax[i];
			pos1y = rows.posay[i++];
			s.xh[rows.id] = rows.xhit[findinfo(pos1x, pos1y)];
			s.yh[rows.id] = rows.yhit[findinfo(pos1x, pos1y)];
			rows.id++;
			printf("dfgdfgdfg(%f)\n", rows.xhit[1]);
			if(rows.xhit[i] < MAXINT - 100)
				break;
		}
		pos2x = rows.posax[i];
		pos2y = rows.posay[i];
		printf("%d|%d\n", pos1x, pos2x);
		if(rows.xhit[i] < MAXINT - 100)
			compare(pos1x, pos1y, pos2x, pos2y);
		// printf("{%f|%f}\n", s.xh[0], s.yh[0]);
		// printf("|%f|%f|\n", s.xh[1], s.yh[1]);
		rows.id++;
	}
	printf("<%f|%f>\n", s.xh[0], s.yh[0]);
	printf("<%f|%f>\n", s.xh[1], s.yh[1]);
}
void	chosePoints()
{
	float a;
	float b;
	float c;
	float d;
	float delta;


	a = sqrtf(powf(p.x - r.cast.verticalx, 2) + powf(p.y - r.cast.verticaly, 2));
	b = sqrtf(powf(p.x - r.cast.horizontalx, 2) + powf(p.y - r.cast.horizontaly, 2));
	if (a <= b)
	{
		r.cast.wallHitx = r.cast.verticalx;
		r.cast.wallHity = r.cast.verticaly;
		r.cast.distance = a;
		delta = a;
	}
	else
	{
		r.cast.wallHitx = r.cast.horizontalx;
		r.cast.wallHity = r.cast.horizontaly;
		r.cast.distance = b * (-1);
		delta = b;
	}
}


void	cast(float rayAngle)
{
	//initrows
	rows.id = 0;
	vorh[rows.id] = 0;
	rows.posax[0] = MAXINT;
	rows.posay[0] = MAXINT;

	r.cast.wallHitx = MAXINT;
	r.cast.wallHity = MAXINT;
	r.cast.horizontalx = MAXINT;
	r.cast.horizontaly = MAXINT;
	r.cast.verticalx = MAXINT;
	r.cast.verticaly = MAXINT;
	//sprite
	s.xh[s.sprite] = MAXINT;
	s.yh[s.sprite] = MAXINT;
	s.xv[r.id][s.sprite] = MAXINT;
	s.xh[s.sprite] = MAXINT;
	s.x[r.id][s.sprite] = MAXINT;
	s.y[r.id][s.sprite] = MAXINT;
	s.dist[r.id][s.sprite] = 0;
	horizontalintersect(rayAngle);
	// s.saveh = s.sprite;
	// s.sprite = 0;
	verticalintersect(rayAngle);
	//printf("|%d|\n", rows.id);
	s.sprite = rows.id;
	if(rows.id)
	{
		rows.id = 0;
		sort();
	}
	// if(s.sprite < s.saveh)
	// 	s.sprite = s.saveh;
	chosePoints();
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
			// if(lines[a][b] == '2')
			// {
			// 	put_square(x, y, 2);
			// }
			// else if(indice > 0)
			// {
			// // 	data[(int )p.x + (int )p.y * sc.w] = 0x000000;
			// // 	//Line;
			// // 	drawRays();
			//  	drawLine(p.rotationAngle, 0xFFFFFF, 0);
			// // 	//drawLine();
			// }
			x+=TILESIZE;
			b++;
		}
		y+=TILESIZE;
		a++;
	}
	if(indice > 0)
	{
	// 	data[(int )p.x + (int )p.y * sc.w] = 0x000000;
	// 	//Line;
	 	drawRays();
	// 	//drawLine(p.rotationAngle, 0x000000);
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
	//printf("%f\n", s.x[13]);
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
    test = 0;
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
	r.id = 0;
	//init params of player
	p.x = 0;
	p.y = 0;
	p.radius = 3;
	p.turnDirection = 0;
	p.walkDirection = 0;
	p.rotationAngle = 0;
	p.moveSpeed = 3;
	p.rotationSpeed = 1 * Pi / 180;
	p.look = 0;
	img = mlx_new_image(mlx_ptr, sc.w, sc.h);
	data = (int*)mlx_get_data_addr(img, &a, &b, &c);
	render(height, width, lines, 0);
	//rendersprite();
//	mlx_hook(win_ptr , 2 , 0 ,  keypress, 0);
//	mlx_hook(win_ptr, 3 , 0 ,  keyreleased,  0);
//	mlx_key_hook()
	mlx_loop_hook(mlx_ptr, update, 0);
 	mlx_loop(mlx_ptr);
	return (0);
}