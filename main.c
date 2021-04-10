#include <present.h>
#include <stdio.h>
#include <epoxy/gl.h>
#include <xmmintrin.h>

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

	augsprite(s3, 0.5, 0.3);
	augsprite(s2, 0.5, 0.5);
	augsprite(s1, 0.5, 0.5);
	mvsprite(s1, 0.2, 0.2);
	mvsprite(s2,0,0);
	mvsprite(s3, 0.5, 0.5);

	float sx = 0.1;
	float sy = 0.1;

	int tile = 0;

	float v1[] = { 1,2,3,4 };
	float v2[] = { 5,6,7,8 };
	float *v3 = malloc(sizeof(float)*32);
	__m128 p1, p2, res;
	
	puts("SIMD starting:");
	p1 = _mm_load_ps(v1);
	p2 = _mm_load_ps(v2);
	res = _mm_add_ps(p1, p2);
	_mm_store_ps(v3, res);
	printf("%f %f %f %f\n", v3[0], v3[1], v3[2], v3[3]);

	for(;;){
		readev();

		glClearColor(0,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		/*if(dm.ev == 1){
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
		}*/
		present();
	}
}
