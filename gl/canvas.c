#include <present.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aux/gl.h>
#include <ctype.h>
#include <aux/cfile.h>
#include <aux/src.h>

extern void mkrect(Canvas *c);
extern void mktex(char *path, int *id);
extern void swapbuf();
extern void setup(float *vert);

int prog;
char *vcode;
char *fcode;
int tex;
float *v;
float sum;

float rect[8] = {
	0.5,0.5,0.5,-0.5,-0.5,-0.5,-0.5,0.5
};

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
	free(vcode);
	free(fcode);
}

void
resized(void)
{
	glViewport(0,0,dm.width,dm.height);
}

Canvas*
canvas(char *path)
{
	puts("canvas");
	Canvas *c = cfile(path);

	mkrect(c);

	dm.c = c;
	return c;
}

void
present(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, dm.c->vb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * dm.c->si * 8, dm.c->vert);
	glBindBuffer(GL_ARRAY_BUFFER, dm.c->tb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * dm.c->si * 8, dm.c->tex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, dm.c->tid);
	glUseProgram(prog);
	glBindVertexArray(dm.c->va);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dm.c->ib);
	glDrawElements(GL_TRIANGLES, dm.c->si*6, GL_UNSIGNED_INT, 0);
	swapbuf();
}

void
freecanvas(Canvas *c)
{
	free(c);
	
	if(prog == 0)
		return;
	else{
		glDeleteProgram(prog);
		free(vcode);
		free(fcode);
		prog = 0;
	}
}
