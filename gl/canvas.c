#include <present.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <epoxy/gl.h>

extern void mkrect(int sn);
extern void mktex(char *path);
extern void swapbuf();

int prog;
char *vcode;
char *fcode;
int va, vb, ib;
int tex;
float *v;
float sum;

static char*
src(char *path)
{
	FILE *fp;
	long size;
	char *buf;

	fp = fopen(path, "rb");
	if(fp == 0){
		perror("fopen");
		exit(-1);
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	buf = (char*)calloc(size, sizeof(char));
	fread(buf, 1, size, fp);
	fclose(fp);

	return buf;
}

void
mkprog(void)
{
	int vs, fs, win;
	char log[512];
	vcode = src(VERT);
	fcode = src(FRAG);

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const char**)&vcode, 0);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &win);
	if(!win){
		glGetShaderInfoLog(vs, 512, 0, log);
		printf("OpenGL: Vertex shader compilation failed:\n%s\n", log);
		exit(-1);
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const char**)&fcode, 0);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &win);
	if(!win){
		glGetShaderInfoLog(vs, 512, 0, log);
		printf("OpenGL: Fragment shader compilation failed:\n%x\n", log[0]);
		exit(-1);
	}

	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &win);
	if(!win){
		glGetProgramInfoLog(prog, 512, 0, log);
		printf("OpenGL: Program linkage failed:\n%s\n", log);
		exit(-1);
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void
resized(void)
{
	glViewport(0,0,dm.width,dm.height);
}

Canvas*
canvas(int sn, int tn, char *tex)
{
	int size = sizeof(Canvas)
		+ sizeof(Sprite) * sn
		+ sizeof(Text) * tn;
	Canvas *c = malloc(size);
	c->ti = 0;
	c->si = 0;
	c->sv = (Sprite*)(c+1);
	c->tv = (Text*)(c->sv+sn);

	mkrect(sn);
	mktex(tex);

	return c;
}

void
setcanvas(Canvas *c)
{
	int i;
	int j;
	float s[c->si];
	for(i=0, sum=0; i<c->si; ++i)
		sum += c->sv[i].v;
	for(i=0; i<c->si; ++i)
		s[i] = c->sv[i].v / sum;
	for(i=0; i<c->si; ++i){
		float sub;
		for(j=0, sub = 0; j<i; ++j)
			sub += s[j];
		c->sv[i].tex[0] = c->sv[i].u / sum;
		c->sv[i].tex[1] = 1 - sub;
		c->sv[i].tex[2] = c->sv[i].u / sum;
		c->sv[i].tex[3] = 1 - sub - c->sv[i].v / sum;
		c->sv[i].tex[4] = 0;
		c->sv[i].tex[5] = 1 - sub - c->sv[i].v / sum;
		c->sv[i].tex[6] = 0;
		c->sv[i].tex[7] = 1 - sub;
	}

	dm.c = c;
}

void
present(void)
{
	float vert[dm.c->si*20];
	v = vert;
	int i;
	for(i=0; i<dm.c->si; ++i)
		memmove(v + i*20, dm.c->sv[i].vert, sizeof(float)*20);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * 20, vert);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, tex);
	glUseProgram(prog);
	glBindVertexArray(va);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glDrawElements(GL_TRIANGLES, dm.c->si*6, GL_UNSIGNED_INT, 0);
	swapbuf();
}

void
freecanvas(Canvas *c)
{
	free(c);
	glDeleteProgram(prog);
	free(vcode);
	free(fcode);
}
