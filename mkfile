<mk/mkconfig
<mk/mkconfig.$os

ofiles=\
	$usr\
	$sys/dormer.o\
	$sys/$wsi/graphics.o\
	$vis/canvas.o\
	$vis/sprite.o\
	spit.o\
	$vis/spit.o\

hfiles=\
	aux/cfile.h\
	aux/ff.h\
	
def=\
	-DVERT=\"$vert\"\
	-DFRAG=\"$frag\"

$targ: $ofiles $hfiles
	$cc -o $targ $ofiles $ln -lm
%.o: %.c
	$cc -c -I. $def -std=c89 $flags $stem.c -o $stem.o	
clean:V:
	rm -f $ofiles *.core
nuke: clean
	rm -f $targ 
