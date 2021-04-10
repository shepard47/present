static FILE *fp;
static short bin;
static int snum = 0;
static int i;
static int len = 0;
static fpos_t pos;
static int c;
static int tnum;

void
getint(int *num)
{
	c = fgetc(fp);
	if(c==' ' || c=='\t' || c=='\n')
		c = fgetc(fp);
	if(isdigit(c)){
		*num *= 10;
		*num += c-'0';
		getint(num);		
	}else{
		ungetc(c, fp);
		return;
	}
}

void
getlen()
{
	c = fgetc(fp);
	if(c==' ' || c=='\t' || c=='\n')
		c = fgetc(fp);
	if(c != ':' && c != ';'){
		len++;
		getlen();
	}
}

char*
label(fpos_t *pos, int len)
{
	fsetpos(fp, pos);
	char *name = (char*)calloc(len, len);
	for(int j=0; j<len; ++j)
		name[j] = fgetc(fp);
	return name;
}

Canvas*
cfile(char *path)
{
	fp = fopen(path, "r");
	bin = fgetc(fp) - '0';
	getint(&snum);
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
		getlen();
		ca->sv[i].label = label(&pos, len);

		tnum = 0;
		fgetc(fp);
		getint(&tnum);
		fgetc(fp);
		ca->sv[i].tnum = tnum;
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
	if(bin){
		/* load image */
	}else{
		do
			fgetc(fp);
		while(c==' ' || c=='\n' || c=='\t');
		fgetpos(fp, &pos);
		len = 0;
		getlen();
		fsetpos(fp, &pos);
		char *path = (char*)calloc(len, len);
		for(int j=0; j<len; ++j)
			path[j] = fgetc(fp);
		ca->texp = path;
	}

	return ca;
}

