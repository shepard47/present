#include <aux/gl.h>

int sva, svb, sib;

void
mkspitrect()
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
