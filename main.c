#include <present.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <aux/gl.h>

Canvas *c;
Canvas *yo;
Sprite *s1, *s2, *s3;

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
	dm.cursor = Ccross;
	winit("present");

	yo = canvas("res/yoo.cb");

	s1 = sprite(yo, "fire.png");
	s2 = sprite(yo, "anim.png");
	s3 = sprite(yo, "hobbit.png");

	transprite(s1, 0, 0, 0.5, 0.5, 1);
	transprite(s2, 0.7, 0.7, 1, 1, 1);
	transprite(s3, -0.7, -0.7, -1, -1, 1);
	
	float sx = 0.1;
	float sy = 0.1;
	int tile = 0;

	dm.c = yo;
	int row = 0;

	int t1, t2;
	t1 = usec();

	for(;;){
		readev();

		glClearColor(0,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);

		if(dm.ev == 1){
			transprite(s2, dm.x, dm.y, sx, sy, 0);
			if(dm.btn == 8){
				sx += 0.05;
				sy += 0.05;
				transprite(s2, dm.x, dm.y, sx, sy, 0);
			}else if(dm.btn == 16){
				sx -= 0.05;
				sy -= 0.05;
				transprite(s2, dm.x, dm.y, sx, sy, 0);
			}
		}else if(dm.ev == 2){
/*
			t2 = usec();
			printf("usec: %ld\n", t2-t1);
			t1 = usec();
*/
			printf("key: %x -> %c\n", dm.key, dm.key);
			if(dm.key == 'l')
				dm.c = c;
			if(dm.key == 'h')
				dm.c = yo;
			if(dm.key == 'j')
				row++;
			if(dm.key == 'k')
				row--;
			if(dm.key == 'q'){
				closed();
				exit(0);
			}
			setsprite(s2, row, tile++);
		}
		present();
	}
}
