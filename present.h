typedef struct Canvas Canvas;
typedef struct Sprite Sprite;
typedef struct Text Text;
typedef struct Face Face;
typedef struct Spit Spit;

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

struct Pit
{
	Spit *sv;
	int sn;
	int smax;
	int cap;
	Face *face;
	int w, h;
	char *stack;	
	int ch;
};

extern struct Dormer dm;
extern struct Pit pit;

struct Canvas
{
	int si;
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
	float w, h;
	float *first;
};

struct Spit
{
	char *text;
	int len;
	int x, y, h;
};

struct Face 
{
	char w, h;
	short num;
	int *ind;
	char *map;
};

enum
{
	Chide, Carrow, Ccross,
	Cload, Ctext, Chand,
	Cno, Chelp, Cmove
};

extern void winit(char *label);
extern void readev(void);
extern void setcurs(int curs);
extern int usec(void);
extern int closed(void);
extern void present(void);
extern Canvas *canvas(char *path);
extern void freecanvas(Canvas *c);
extern void mkprog();
extern Sprite *sprite(Canvas *c, char *label);
extern void transprite(Sprite *s, float x, float y, float sx, float sy, float a);
extern void setsprite(Sprite *s, int row, int tile);
extern Text *text(Canvas *c, char *s);
extern void mvtext();
extern void spinit(Face *face, int smax);
extern Spit *newspit(char *text);
extern void transpit(Spit *s, int x, int y, int h);
extern void altspit(Spit *s, char *text); 
extern void rmspit(void);
extern void spit(void);
