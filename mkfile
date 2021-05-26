<mkconfig

ofiles=\
	$usr\
	$sys/dormer.o\
	$wsi/graphics.o\
	$vis/canvas.o\
	$vis/sprite.o\
	
def=\
	-DVERT=\"$vert\"\
	-DFRAG=\"$frag\"

$targ: $ofiles
	$cc -o $targ $ofiles $ln -lm
%.o: %.c
	$cc -c -I. $def -std=c89 $flags $stem.c -o $stem.o	
clean:V:
	rm -f $ofiles
nuke: clean
	rm -f $targ
