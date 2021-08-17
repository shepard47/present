static FILE *fp;
static int snum = 0;
static int i;

extern void mktex(Canvas *c, int *id);

static void
getint(int *num, int *c)
{
	*c = fgetc(fp);
	if(*c==' ' || *c=='\t' || *c=='\n')
		*c = fgetc(fp);
	if(isdigit(*c)){
		*num *= 10;
		*num += *c-'0';
		getint(num, c);
	}else{
		ungetc(*c, fp);
		return;
	}
}

static void
getlen(int *len, int *c)
{
	*c = fgetc(fp);
	if(*c==' ' || *c=='\t' || *c=='\n'){
		*c = fgetc(fp);
	}
	if(*c != ':' && *c != ';'){
		(*len)++;
		getlen(len, c);
	}
}

static char*
label(fpos_t *pos, int len)
{
	fsetpos(fp, pos);
	char *name = (char*)calloc(len, len);
	int j;
	for(j=0; j<len; ++j)
		name[j] = fgetc(fp);
	return name;
}

Canvas*
cfile(char *path)
{
	int c;
	int tnum = 0;
	int rnum = 0;
	snum = 0;
	int len = 0;
	fpos_t pos;
	fp = fopen(path, "r");


	c = fgetc(fp);
	if(c != 'f'){
		printf("incorrect canvas file: %s\n", path);
		exit(-1);
	}
	char magic[8];
	int width, height;
	long size;
	short *data;	

	ungetc(c, fp);	
	fread(magic, 1, 8, fp);
	if(strncmp(magic, "farbfeld", 8) != 0){
		puts("someone here is missing");
		exit(-1);
	}
	fread(&width, 1, 4, fp);
	fread(&height, 1, 4, fp);
	width = htonl(width);
	height = htonl(height);

	size = width * height * 8;
        data = (short*)malloc(size);
        if(data == 0){
		printf("allocation failed: %s\n", path);
		exit(-1);	
	}
        size = fread(data, 2, size/2, fp);
        if(size != width * height * 4){
                free(data);
		printf("read failed: %s\n", path);
		exit(-1);
        }
/*
	fseek(fp, 16 + width * height * 8, SEEK_SET);
*/

	getint(&snum, &c);

	Canvas *ca = (Canvas*)malloc(sizeof(Canvas)
		+ sizeof(Sprite) * snum
		+ sizeof(float)*8*snum
		+ sizeof(float)*8*snum);
	ca->sv = (Sprite*)(ca + 1);
	ca->vert = (float*)(ca->sv + snum);
	ca->tex = (float*)(ca->vert + snum*8);
	ca->si = snum;
	puts("canvas fully allocated");

	for(i=0; i<snum; ++i){
		puts("loop");
		do
			c = fgetc(fp);
		while(c==' ' || c=='\t' || c=='\n');
		ungetc(c, fp);
		fgetpos(fp, &pos);
		len = 0;
		getlen(&len, &c);
		ca->sv[i].label = label(&pos, len);

		tnum = 0;
		fgetc(fp);
		getint(&tnum, &c);
		fgetc(fp);
		ca->sv[i].tnum = tnum;

		rnum = 0;
		fgetc(fp);
		getint(&rnum, &c);
		fgetc(fp);
		ca->sv[i].rnum = rnum;

		ca->sv[i].tex = ca->tex + i*8;
		fscanf(fp, "%f %f %f %f %f %f %f %f",
			&ca->sv[i].tex[0],
			&ca->sv[i].tex[1],
			&ca->sv[i].tex[2],
			&ca->sv[i].tex[3],
			&ca->sv[i].tex[4],
			&ca->sv[i].tex[5],
			&ca->sv[i].tex[6],
			&ca->sv[i].tex[7]);
		puts("end of loop");
	}

	ca->w = width;
	ca->h = height;
	ca->data = data;

	fclose(fp);
	puts("into mktex:");
	mktex(ca, &ca->tid);
	return ca;
}
