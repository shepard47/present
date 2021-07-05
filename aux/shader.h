char *svert = {
	"#version 330 core\n"
	"\n"
	"layout (location = 0) in vec2 loc;\n"
	"layout (location = 1) in vec2 tex;\n"
	"\n"
	"out vec2 texcoord;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(loc, 0.0, 1.0);\n"
	"	texcoord = tex;\n"
	"}\n"
	"\n"
	"\n"
};

char *sfrag = {
	"#version 330 core\n"
	"\n"
	"out vec4 col;\n"
	"in vec2 texcoord;\n"
	"\n"
	"uniform sampler2D tex;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	col = texture(tex, texcoord);\n"
	"}\n"
	"\n"
};

