#include <present.h>
#include <stdlib.h>
#include <string.h>

extern void mkspitrect(void);
extern void mkspittex(void);
extern void mkspitshad(void);

struct Pit pit;

void
spinit(Face *face, int smax)
{
	pit.stack = (char*)calloc(1047, sizeof(char));	
	pit.sv = (Spit*)malloc(sizeof(Spit) * smax);
	pit.smax = smax;
	pit.cap = 1047;
	pit.ch = 0;
	pit.sn = 0;

	mkspitrect();
/*
	mkspittex();
	mkspitshad();
*/
}

Spit*
newspit(char *text)
{
	Spit *s;

	if(pit.sn < pit.smax)
		s = &pit.sv[pit.sn++];
	else
		return &pit.sv[0];		

	s->len = strlen(text);	
	if(s->len+pit.ch < pit.cap){
		memmove(&pit.stack[pit.ch], text, s->len); 
		s->text = &pit.stack[pit.ch];
		pit.ch += 1 + s->len;
	}else{
		pit.stack = (char*)realloc(pit.stack, sizeof(char)*(pit.cap+1000));	
		pit.cap += 1000;
		memmove(&pit.stack[pit.ch], text, s->len);
		int i;
		pit.ch = 0;
		for(i=0; i<pit.sn; ++i){
			pit.sv[i].text = &pit.stack[pit.ch];
			pit.ch += pit.sv[i].len;
		}	
		pit.ch += 1 + s->len;
	}

	return s;
}

void
transpit(Spit *s, int x, int y, int h)
{
	s->x = x;
	s->y = y;
	s->h = h;
}

void
altspit(Spit *s, char *text)
{
	s->text = text;
	
	/* setup texture coordinates */
}

void
rmspit(void)
{
	pit.ch -= 1 + pit.sv[pit.sn].len;
	pit.sn--;	
}

void
spit(void)
{
}

