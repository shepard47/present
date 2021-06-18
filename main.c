#include <present.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <aux/gl.h>

Canvas *c;
Canvas *yo;
Sprite *s1, *s2, *s3, *s4, *s5, *s6, *s7;

int
closed(void)
{
	puts("closing...");
	freecanvas(c);
	freecanvas(yo);
	return 1;
}

int
main()
{
	dm.width = 800;
	dm.height = 600;
	dm.cursor = Ccross;
	dm.screen = 0;
	winit("present");

	yo = canvas("res/tset.ca");

	printf("%d\n", yo->si);
	s4 = sprite(yo, "fire.png");
	s5 = sprite(yo, "char.png");
	s6 = sprite(yo, "hobbit.png");
/*
	s7 = sprite(yo, "nazi.gif");
*/

	transprite(s4, 0, 0, 0.5, 0.5, 1);
	transprite(s5, 0.7, 0.7, 1, 1, 1);
	transprite(s6, -0.7, -0.7, -1, -1, 1);
/*
	//transprite(s7, -0.5, -0.5, 0.1, 0.1, 1);
*/

	c = canvas("res/test.ca");
	s1 = sprite(c, "character");
	s2 = sprite(c, "npc");
	s3 = sprite(c, "background");

	transprite(s1, -0.7, -0.7, 0.5, 0.5, 2.5);
	transprite(s2, 0, 0, 0.5, 0.5, 1);
	transprite(s3, 0.5, 0.5, 0.5, 0.3, 3.14);

	float sx = 0.1;
	float sy = 0.1;
	int tile = 0;

	for(;;){
		readev();

		glClearColor(0,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);

		if(dm.ev == 1){
			transprite(s1, dm.x, dm.y, sx, sy, 0);
			if(dm.btn == 8){
				sx += 0.05;
				sy += 0.05;
				transprite(s1, dm.x, dm.y, sx, sy, 0);
			}else if(dm.btn == 16){
				sx -= 0.05;
				sy -= 0.05;
				transprite(s1, dm.x, dm.y, sx, sy, 0);
			}
		}else if(dm.ev == 2){
			if(dm.key == 'l')
				dm.c = c;
			if(dm.key == 'h')
				dm.c = yo;
			if(dm.key == 'q'){
				closed();
				exit(0);
			}
			setsprite(s1, 1, tile++);
		}
		present();
	}
}
