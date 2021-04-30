#include <present.h>
#include <epoxy/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aux/ff.h>
#include <xmmintrin.h>
#include <math.h>

extern float sum;
extern int va, vb, ib, tb;
extern float rect[8];

void
mkrect(int sn)
{
	int i;
	int ind[sn*6];

	for(i=0; i<sn; ++i){
		ind[i*6    ] = 0 + i*4;
		ind[i*6 + 1] = 1 + i*4;
		ind[i*6 + 2] = 3 + i*4;
		ind[i*6 + 3] = 1 + i*4;
		ind[i*6 + 4] = 2 + i*4;
		ind[i*6 + 5] = 3 + i*4;
	}

	glGenVertexArrays(1, &va);
	glGenBuffers(1, &vb);
	glGenBuffers(1, &ib);
	glGenBuffers(1, &tb);
	glBindVertexArray(va);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sn * 8, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sn * 8, 0, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void
mktex(char *path, int *id)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	unsigned char *data;
	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, chan;
	data = ff(path, &width, &height);
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		printf("Failed to load texture: %s\n", path);
		exit(-1);
	}
}

void
setup(float *vert)
{
	vert[0] = 0.5;
	vert[1] = 0.5;
	vert[2] = 0.5;
	vert[3] = -0.5;
	vert[4] = -0.5;
	vert[5] = -0.5;
	vert[6] = -0.5;
	vert[7] = 0.5;
}

Sprite*
sprite(Canvas *c, char *label)
{
	for(int i=0; i<c->si; ++i){
		if((strcmp(label, c->sv[i].label)) == 0){
			c->sv[i].ind = i;
			c->sv[i].vert = c->vert + i*8;
			setup(c->sv[i].vert);
			c->sv[i].c = c;
			c->sv[i].w = c->sv[i].tex[0] - c->sv[i].tex[4];
			c->sv[i].first = malloc(sizeof(float)*8);
			memmove(c->sv[i].first, c->sv[i].tex, sizeof(float)*8);
			return &c->sv[i];
		}
	}

	printf("sprite: no sprite called %s was found.\n", label);
	exit(-1);
}

void
transprite(Sprite *s, float x, float y, float sx, float sy, float a)
{
	__m128 t,r,v;

	/* rotate SIMD */
	for(int i=0; i<4; ++i){
		s->vert[i*2] = rect[i*2] * cos(a) - rect[i*2+1] * sin(a);
		s->vert[i*2+1] = rect[i*2] * sin(a) + rect[i*2+1] * cos(a);
	}

	/* scale */
	t = _mm_load_ps((float[]){sx,sy,sx,sy});
	r = _mm_loadu_ps(s->vert);
	v = _mm_mul_ps(t,r);
	_mm_storeu_ps(s->vert, v);
	r = _mm_loadu_ps(s->vert+4);
	v = _mm_mul_ps(t,r);
	_mm_storeu_ps(s->vert+4, v);

	/* move */
	t = _mm_load_ps((float[]){x,y,x,y});
	r = _mm_loadu_ps(s->vert);
	v = _mm_add_ps(t,r);
	_mm_storeu_ps(s->vert, v);
	r = _mm_loadu_ps(s->vert+4);
	v = _mm_add_ps(t,r);
	_mm_storeu_ps(s->vert+4, v);
}


void
setsprite(Sprite *s, int tile)
{
	/* SSE maybe? */
	for(int i=0; i<4; ++i)
		s->tex[i*2] = s->first[i*2] + tile * s->w;
}
