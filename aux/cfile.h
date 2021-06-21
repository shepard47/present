static FILE *fp;
static int snum = 0;
static int i;

extern void mktex(char *path, int *id);

static int
swapped(int num)
{
	int le = ((num>>24)&0xff) |
				((num<<8)&0xff0000) |
				((num>>8)&0xff00) |
				((num<<24)&0xff000000);
	return le;
}

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
	int bin = 0;
	int tnum = 0;
	int rnum = 0;
	snum = 0;
	int len = 0;
	fpos_t pos;
	fp = fopen(path, "r");


	c = fgetc(fp);
	if(c == 'f'){
		bin = 1;
		char magic[8];
		int width, height;
		ungetc(c, fp);	
		fread(magic, 1, 8, fp);
		if(strncmp(magic, "farbfeld", 8) != 0){
			puts("someone here is missing");
			exit(-1);
		}
		fread(&width, 1, 4, fp);
		fread(&height, 1, 4, fp);
		fseek(fp, 16 + swapped(width) * swapped(height) * 8, SEEK_SET);
	}
	else
		ungetc(c, fp);		


	getint(&snum, &c);

	Canvas *ca = (Canvas*)malloc(sizeof(Canvas)
		+ sizeof(Sprite) * snum
		+ sizeof(float)*8*snum
		+ sizeof(float)*8*snum);
	ca->sv = (Sprite*)(ca + 1);
	ca->vert = (float*)(ca->sv + snum);
	ca->tex = (float*)(ca->vert + snum*8);
	ca->si = snum;

	for(i=0; i<snum; ++i){
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
	}

	if(bin)
		ca->texp = path;
	else{	
		do
			fgetc(fp);
		while(c==' ' || c=='\n' || c=='\t');
		fgetpos(fp, &pos);
		len = 0;
		getlen(&len, &c);
		fsetpos(fp, &pos);
		char *texp = (char*)calloc(len, len);
		int j;
		for(j=0; j<len; ++j)
		texp[j] = fgetc(fp);
		ca->texp = texp;
	}

	fclose(fp);
	mktex(ca->texp, &ca->tid);
	return ca;
}
