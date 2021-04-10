#include <present.h>
#include <stdio.h>
#include <epoxy/gl.h>

Canvas *c;
Sprite *s1;
Sprite *s2;
Sprite *s3;

int
closed(void)
{
	puts("closing...");
	freecanvas(c);
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
	
	c = canvas("res/test.ca");
	s1 = sprite(c, "character");
	s2 = sprite(c, "npc");
	s3 = sprite(c, "background");

	transprite(s1, -0.7, -0.7, 0.5, 0.5, 0);
	transprite(s2, 0, 0, 0.5, 0.5, 0);
	transprite(s3, 0.5, 0.5, 0.5, 0.3, 0);

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
		}
		present();
	}
}
