/* move farbfeld here */

static FILE *fp;

static int
swapped(int num) {
	int le = ((num>>24)&0xff) |
				((num<<8)&0xff0000) |
				((num>>8)&0xff00) |
				((num<<24)&0xff000000);
	return le;
}

static void
fail(char *func)
{
	perror(func);
	exit(-1);
}

void*
ff(char *path, int *width, int *height)
{
	char magic[8];
	short *data;
	int size;
	fp = fopen(path, "r");
	if(fp == 0)
		fail("fopen");
	if(fread(magic, 1, 8, fp) != 8)
		fail("fread(magic)");
	if(strncmp(magic, "farbfeld", 8) != 0)
		fail("strcmp(magic, farbfeld)");
	if(fread(width, 1, 4, fp) != 4)
		fail("fread(width)");
	if(fread(height, 1, 4, fp) != 4)
		fail("fread(height)");
	*width = swapped(*width);
	*height = swapped(*height);

	size = (*width) * (*height) * 8;
	data = (short*)malloc(size);
	if(data == 0)
		fail("malloc");
	size = fread(data, 2, size/2, fp);
	/* bad size on windows (?) */
	/*if(size != (*width) * (*height) * 4){
		free(data);
		fail("fread(data)");
	}*/
	fclose(fp);
	return data;
}

