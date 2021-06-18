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
};

extern struct Dormer dm;

struct Canvas
{
	int si, ti;
	Sprite *sv;
	char *texp;
	float *vert;
	float *tex;
	int tid;
	int va, vb, ib, tb;
};

struct Sprite
{
	Canvas *c;
	int ind;
	char *label;
	float *vert;
	int tnum;
	int rnum;
	int tile;
	float x,y;
	float *tex;
	float w;
	float *first;
};

struct Text
{
	Canvas *c;
	char *s;
	int len;
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
extern Canvas *canvas(char *path);
extern void freecanvas(Canvas *c);
extern void mkprog();
/* sprite.c */
extern Sprite *sprite(Canvas *c, char *label);
extern void transprite(Sprite *s, float x, float y, float sx, float sy, float a);
extern void setsprite(Sprite *s, int tile);
/* text.c */
extern Text *text(Canvas *c, char *s);
extern void mvtext();
