#include <present.h>
#include <epoxy/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aux/ff.h>

extern int tex;
extern float sum;
extern int va, vb, ib, tb;

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
	glBindVertexArray(va);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 20, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void
mktex(char *path)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	unsigned char *data;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
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

Sprite*
sprite(Canvas *c, char *label)
{
	for(int i=0; i<c->si; ++i){
		if((strcmp(label, c->sv[i].label)) == 0){
			c->sv[i].ind = i;
			c->sv[i].vert = c->vert + i*20;
			return &c->sv[i];
		}
	}

	printf("sprite: no sprite called %s was found.\n", label);
	exit(-1);
}

void
mvsprite(Sprite *s, float x, float y)
{
	s->vert[0] = x + s->sx;
	s->vert[1] = y + s->sy;
	s->vert[2] = 0;
	s->vert[3] = s->tex[0];
	s->vert[4] = s->tex[1];
	s->vert[5] = x + s->sx;
	s->vert[6] = y - s->sy;
	s->vert[7] = 0;
	s->vert[8] = s->tex[2];
	s->vert[9] = s->tex[3];
	s->vert[10] = x - s->sx;
	s->vert[11] = y - s->sy;
	s->vert[12] = 0;
	s->vert[13] = s->tex[4];
	s->vert[14] = s->tex[5];
	s->vert[15] = x - s->sx;
	s->vert[16] = y + s->sy;
	s->vert[17] = 0;
	s->vert[18] = s->tex[6];
	s->vert[19] = s->tex[7];
}

void
augsprite(Sprite *s, float sx, float sy)
{
	s->sx = sx;
	s->sy = sy;
}

/*void
setsprite(Sprite *s, int tile)
{
	s->tex[0] = (s->u / sum)*(tile + 1);
	s->tex[2] = (s->u / sum)*(tile + 1);
	s->tex[4] = (s->u / sum)*tile;
	s->tex[6] = (s->u / sum)*tile;
}*/
