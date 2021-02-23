#include <present.h>
#include <stdio.h>

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

	c = canvas(3,2, "res/texture.png");
	s1 = sprite(c, 0.1, 0.1);
	s2 = sprite(c, 0.1, 0.1);
	s3 = sprite(c, 0.3, 0.3);
	setcanvas(c);

	mvsprite(s1, 0.2, 0.2);
	mvsprite(s2,-0.5,-0.5);
	mvsprite(s3, 0.5, 0.5);

	float sx = 0.1;
	float sy = 0.1;

	int tile = 0;

	for(;;){
		readev();
		if(dm.ev == 1){
			mvsprite(s1, dm.x, dm.y);
			if(dm.btn == 8){
				sx += 0.05;
				sy += 0.05;
				augsprite(s1, sx, sy);
				mvsprite(s1, dm.x, dm.y);
			}else if(dm.btn == 16){
				sx -= 0.05;
				sy -= 0.05;
				augsprite(s1, sx, sy);
				mvsprite(s1, dm.x, dm.y);
			}
		}else if(dm.ev == 2){
			setsprite(s1, tile++);
			if(tile >= 4)
				tile = 0;
			mvsprite(s1, dm.x, dm.y);
		}
		present();
	}
}
