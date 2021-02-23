typedef struct Canvas Canvas;
typedef struct Sprite Sprite;
typedef struct Text Text;

struct Dormer
{
	void *dis;
	void *win;
	int ev;
	int key;
	int btn;
	float x, y;
	int width;
	int height;
	int screen;
	int cursor;
	Canvas *c;	
} dm;

struct Canvas
{
	int si, ti;
	Sprite *sv;
	Text *tv;
};

struct Sprite
{
	Canvas *c;
	float vert[20];
	int tnum;
	int tile;
	float x,y;
	float sx,sy;
	float u,v;
	float tex[8];
};

struct Text
{
	Canvas *c;
	void *im;
	void *fg;
	void *ft;
	char *str;
	int width;
	float x, y;
};

enum
{
	Chide, Carrow, Ccross,
	Cload, Ctext, Chand,
	Cno, Chelp, Cmove
};

/* dormer.c */
extern void winit(char *label);
extern void readev(void);
extern void setcurs(int curs);
/* main.c */
extern int closed(void);
/* canvas.c */
extern void present(void);
extern Canvas *canvas(int sn, int tn, char *tex);
extern void setcanvas(Canvas *c);
extern void freecanvas(Canvas *c);
extern void mkprog();
/* sprite.c */
extern Sprite *sprite(Canvas *c, float sx, float sy);
extern void mvsprite(Sprite *s, float x, float y);
extern void augsprite(Sprite *s, float sx, float sy);
extern void setsprite(Sprite *s, int tile);
/* text.c */
extern void mktext();
extern void mvtext();