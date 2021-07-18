#include <present.h>
#include <aux/gl.h>
#include <aux/spit.h>

extern Face spleen;

int sva, svb, sib;
int stex;

void
mkspitrect(void)
{
	float vert[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f 
	};
	int ind[] = { 
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &sva);
	glGenBuffers(1, &svb);
	glGenBuffers(1, &sib);

	glBindVertexArray(sva);
	glBindBuffer(GL_ARRAY_BUFFER, svb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void
mkspittex(void)
{
	glGenTextures(1, &stex);	
	glBindTexture(GL_TEXTURE_2D, stex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, spleen.w, spleen.h, 0, GL_RED, GL_UNSIGNED_BYTE, spleen.map);
	glGenerateMipmap(GL_TEXTURE_2D);
	
}
