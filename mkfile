<mk/mkconfig

usr=main.o aux/gl.o

ofiles=\
	$usr\
	$sys/dormer.o\
	$sys/$wsi/graphics.o\
	$vis/canvas.o\
	$vis/sprite.o
hfiles=\
	aux/cfile.h\
	aux/ff.h\
	aux/gl.h

$targ: $ofiles $hfiles
	$cc -o $targ $ofiles $ln -lm
%.o: %.c
	$cc -c -I. -std=c89 $flags $stem.c -o $stem.o	
clean:V:
	rm -f $ofiles *.core
nuke: clean
	rm -f $targ 
