char*
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

