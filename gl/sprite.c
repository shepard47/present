#include <present.h>
#include <epoxy/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stbi.h>

extern int va, vb, ib;
extern int tex;
extern float sum;

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
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(path, &width, &height, &chan, 0);
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		printf("STBi: Failed to load texture:\n%s\n", path);
		exit(-1);
	}
	stbi_image_free(data);
}

Sprite*
sprite(Canvas *c, float sx, float sy)
{
	Sprite *s = &c->sv[c->si++];
	s->sx = sx;
	s->sy = sy;
	s->u = sx;
	s->v = sy;
	return s;
}

void
mvsprite(Sprite *s, float x, float y)
{
	float vert[] = {
		x + s->sx, y + s->sy, 0, s->tex[0], s->tex[1],
		x + s->sx, y - s->sy, 0, s->tex[2], s->tex[3],
		x - s->sx, y - s->sy, 0, s->tex[4], s->tex[5],
		x - s->sx, y + s->sy, 0, s->tex[6], s->tex[7],
	};
	memmove(s->vert, vert, sizeof(float) * 20);
}

void
augsprite(Sprite *s, float sx, float sy)
{
	s->sx = sx;
	s->sy = sy;
}

void
setsprite(Sprite *s, int tile)
{
	s->tex[0] = (s->u / sum)*(tile + 1);
	s->tex[2] = (s->u / sum)*(tile + 1);
	s->tex[4] = (s->u / sum)*tile;
	s->tex[6] = (s->u / sum)*tile;
}
