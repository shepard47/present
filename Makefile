include mkconfig

TARG=pres
OFILES=\
	main.o\
	$(sys)/dormer.o\
	$(wsi)/graphics.o\
	$(vis)/canvas.o\
	$(vis)/sprite.o\
	#$(vis)/text.o\

DEF=\
	-DVERT=\"$(vert)\"\
	-DFRAG=\"$(frag)\"

TARG: $(OFILES)
	$(cc) -o $(TARG) $(OFILES) -Wall $(ln)
%.o: %.c
	$(cc) -c $^ -o $@ -I. $(DEF)
