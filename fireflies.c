#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "gfx.h"

void draw_zero(int x, int y, int width, int height);
void draw_one(int x, int y, int width, int height);
void draw_two(int x, int y, int width, int height);
void draw_three(int x, int y, int width, int height);
void draw_four(int x, int y, int width, int height);
void draw_five(int x, int y, int width, int height);
void draw_six(int x, int y, int width, int height);
void draw_seven(int x, int y, int width, int height);
void draw_eight(int x, int y, int width, int height);
void draw_nine(int x, int y, int width, int height);

void draw_A(int x, int y, int width, int height);
void draw_C(int x, int y, int width, int height);
void draw_D(int x, int y, int width, int height);
void draw_E(int x, int y, int width, int height);
void draw_F(int x, int y, int width, int height);
void draw_H(int x, int y, int width, int height);
void draw_I(int x, int y, int width, int height);
void draw_L(int x, int y, int width, int height);
void draw_M(int x, int y, int width, int height);
void draw_N(int x, int y, int width, int height);
void draw_O(int x, int y, int width, int height);
void draw_P(int x, int y, int width, int height);
void draw_R(int x, int y, int width, int height);
void draw_S(int x, int y, int width, int height);
void draw_T(int x, int y, int width, int height);
void draw_Y(int x, int y, int width, int height);

void draw_slash(int x, int y, int width, int height);
void draw_plus(int x, int y, int width, int height);
void draw_string(char *s, int xc, int yc, int width, int height);
void draw_numeral(int number, int x, int y, int width, int height);
void draw_score(int score);
void draw_time(long micro_seconds);
void draw_play_button(int x, int y);

struct firefly{
	double xpos, ypos, v;
};

void draw_firefly(int x, int y);

void draw_circle(double xc, double yc, double r);
void draw_p1(double xc, double yc, double r1, double r2, double phase);
void draw_p2(double xc, double yc, double phase, double pointvalue);
void draw_arc(double xc, double yc, double theta, double phi, double r);
void draw_ship(double direction, double xc, double yc);

void draw_intro_screen(double phase);
void draw_end_screen(double score, long time);
void eval_random(long time, int *p1x, int *ep1, int *p1y, int *p2x, int *p2y, int *ep2, int *pointvalue);
void spawn_firefly();
int is_p1_collision(int *p1x, int *p1y, int playerX, int playerY, int *ep1, long time, long *t_init, int *ex, int *ey, struct firefly all[500], double *score, int *fireflies_k);
int is_p2_collision(int *p2x, int *p2y, int playerX, int playerY, int *ep2, int *pointvalue, double *score);
void draw_explosion(int xc, int yc, long t_init, long t_current);

double process_keys(char input);

void move_player(double dt, double direction, double *xp, double *yp, double vmax);

void add_fireflies(struct firefly all[500], double xp, double yp);
void move_firefly(struct firefly all[500], struct firefly *one, double xp, double yp, double dt);
void move_allfireflies(struct firefly all[500], double xp, double yp, double dt);
int check_firefly(double direction, double xc, double yc, struct firefly all[100]);
int check_fcollision(struct firefly all[500], double xc, double yc, double direction);
int check_line(double x1, double y1, double x2, double y2, double x, double y);

void add_powerup(struct firefly *pu, double xp, double yp, double phase);
void shift_array(struct firefly all[500], int i);
void clear_fireflies(struct firefly all[500], double xp, double yp, double *score, int *fireflies_k);
void check_pcollision(struct firefly *pu, double xp, double yp, double direction,struct firefly all[500]);

int main (void) {
    	gfx_open(500,500,"Fireflies");
    	long t = 0;
    	double xp = 250;
    	double yp = 250;
    	double vmax = 75;
    	double dt = 10000;
    	double phase = 0;
    	double score = 0;
    	double direction = 0;
    	char input = 0;
    	int isInput = 0;
    	int startInput = 0;
    	int xcursor = 0;
    	int ycursor = 0;
    	int toPlay = 0;

    	int xp1 = 0;
    	int yp1 = 0;
    	int xp2 = 0;
    	int yp2 = 0;

    	int ep1 = 0;
    	int ep2 = 0;

    	int is_p1c = 0;
    	int is_p2c = 0;

    	int pointvalue = 0;

    	int fireflies_k = 0;

    	long t_init = 0;
    	int ex = 0;
    	int ey = 0;

    	struct firefly all[500];
    	int i;
    	for(i=0; i<500; i++){
            	all[i].v=0;
    	}

    	struct firefly pu;
    	pu.v=0;

    	srand(time(NULL));
    	while(toPlay == 0) {
            	isInput = gfx_event_waiting();
            	if(isInput) {
                    	gfx_wait();
                    	xcursor = gfx_xpos();
                    	ycursor = gfx_ypos();
                    	if(xcursor >= 192 && xcursor <= 302 && ycursor <= 380 && ycursor >= 320) {
                            	break;
                    	}
            	}
            	gfx_clear();
            	draw_intro_screen(phase);
            	gfx_flush();
            	usleep(dt);
            	t += dt;
            	phase += (M_PI / 5) * 0.01;
            	isInput = 0;
            	input = 0;
    	}

    	t = 0;
    	phase = 0;

    	while(1) {
            	isInput = gfx_event_waiting();
            	if(isInput) {
                    	input = gfx_wait();
                    	direction = process_keys(input);
            	}
            	usleep(dt);
            	t += dt;
            	phase += (M_PI / 5) * 0.01;
            	move_player(dt,direction,&xp,&yp,vmax);
            	gfx_clear();

            	draw_time(t);
            	score += dt / 1000000;
            	draw_score((int)score);
            	draw_ship(direction,xp,yp);
            	eval_random(t,&xp1,&yp1,&ep1,&xp2,&yp2,&ep2,&pointvalue);

            	if(ep1) {
                    	draw_p1(xp1,yp1,20,3,phase);
                    	is_p1c = is_p1_collision(&xp1,&yp1,xp,yp,&ep1,t,&t_init,&ex,&ey,all,&score,&fireflies_k);
            	}

            	if(ep2) {
                    	draw_p2(xp2,yp2,phase * 4,pointvalue);
                    	is_p2c = is_p2_collision(&xp2,&yp2,xp,yp,&ep2,&pointvalue,&score);
            	}

            	if(ex != 0) draw_explosion(ex,ey,t_init,t);

            	add_fireflies(all,xp,yp);
            	move_allfireflies(all, xp, yp, dt);
            	if(check_fcollision(all, xp, yp, direction)) break;
            	//check_pcollision(&pu, xp, yp, direction, all);
            	gfx_flush();

            	if(xp > 500 || xp < 0 || yp > 500 || yp < 0) break;

            	isInput = 0;
            	input = 0;
    	}

    	toPlay = 0;
    	printf("Fireflies eliminated: %d\n", fireflies_k);

    	while(toPlay == 0) {
            	isInput = gfx_event_waiting();
            	if(isInput) {
                    	gfx_wait();
                    	xcursor = gfx_xpos();
                    	ycursor = gfx_ypos();
                    	if(xcursor >= 192 && xcursor <= 302 && ycursor <= 380 && ycursor >= 320) {
                            	break;
                    	}
            	}

            	usleep(10000);
            	gfx_clear();
            	draw_end_screen(score,t);
            	gfx_flush();
    	}
}

void draw_time(long micro_seconds) {
    	char s[5] = "TIME";
    	int seconds = micro_seconds / 1000000;
    	int thousands = seconds / 1000;
    	int hundreds = (seconds % 1000) / 100;
    	int tens = (seconds % 100) / 10;
    	int ones = (seconds % 10);
    	draw_string(s,5,5,10,20);
    	gfx_color(0,255,0);
    	draw_numeral(thousands,90,5,10,20);
    	draw_numeral(hundreds,105,5,10,20);
    	draw_numeral(tens,120,5,10,20);
    	draw_numeral(ones,135,5,10,20);
}

void draw_score(int score) {
    	char s[6] = "score";
    	int tenK = score / 10000;
    	int Ks = (score % 10000) / 1000;
    	int Hs = (score % 1000) / 100;
    	int Ts = (score % 100) / 10;
    	int Os = (score % 10);
    	draw_string(s,300,5,10,20);
    	gfx_color(0,255,0);
    	draw_numeral(tenK,400,5,10,20);
    	draw_numeral(Ks,415,5,10,20);
    	draw_numeral(Hs,430,5,10,20);
    	draw_numeral(Ts,445,5,10,20);
    	draw_numeral(Os,460,5,10,20);
}

void draw_play_button(int x, int y) {
    	char s[5] = "play";
    	draw_string(s,x - 20, y + 5, 20, 40);
    	gfx_color(0,255,0);
    	gfx_line(x, y, x + 105, y);
    	gfx_line(x, y + 50, x + 105, y + 50);
    	gfx_line(x, y, x, y + 50);
    	gfx_line(x + 105, y, x + 105, y + 50);
    	gfx_line(x - 5, y - 5, x + 110, y - 5);
    	gfx_line(x - 5, y - 5, x - 5, y + 55);
    	gfx_line(x - 5, y - 5, x, y);
    	gfx_line(x - 5, y + 55, x + 110, y + 55);
    	gfx_line(x - 5, y + 55, x, y + 50);
    	gfx_line(x + 110, y - 5, x + 110, y + 55);
    	gfx_line(x + 110, y - 5, x + 105, y);
    	gfx_line(x + 110, y + 55, x + 105, y + 50);
}

void draw_intro_screen(double phase) {
    	char title[11] = "fireflies";
    	//char prog[30] = "by peter forbes / dinh do";
    	draw_string(title,0,50,40,80);
    	//draw_string(prog,10,470,10,20);
    	draw_p1(250,250,50,7,phase);
    	draw_play_button(197,325);

    	draw_p2(50,200,phase * 4, 25);
    	draw_two(75,190,10,20);
    	draw_five(88,190,10,20);

    	draw_p2(50,300,phase * 4, 50);
    	draw_five(75,290,10,20);
    	draw_zero(88,290,10,20);

    	draw_p2(450, 200, phase * 4, 100);
    	draw_one(390,190,10,20);
    	draw_zero(403,190,10,20);
    	draw_zero(416,190,10,20);

    	draw_p2(450, 300, phase * 4, 200);
    	draw_two(390,290,10,20);
    	draw_zero(403,290,10,20);
    	draw_zero(416,290,10,20);
}

void draw_end_screen(double score, long time) {
    	char s1[10] = "Game Over";
    	draw_string(s1,50,100,30,60);
    	draw_score(score);
    	draw_time(time);
}

void eval_random(long time, int *p1x, int *p1y, int *ep1, int *p2x, int *p2y, int *ep2, int *pointvalue) {
    	int to_spawn_p1 = (int)rand() % 20000;
    	int to_spawn_p2 = (int)rand() % 10000;
    	int pointroll = rand() % 100;

    	if(to_spawn_p1 < 10 && *ep1 == 0) {
            	*ep1 = 1;
            	*p1x = (int)rand() % 500;
            	*p1y = 50 + (int)rand() % 450;
    	}

    	if(to_spawn_p2 > 9990 && *ep2 == 0) {
            	*ep2 = 1;
            	*p2x = (int)rand() % 500;
            	*p2y = 50 + (int)rand() % 450;
            	if(pointroll < 50) *pointvalue = 25;
            	if(pointroll >= 50 && pointroll < 75) *pointvalue = 50;
            	if(pointroll >= 75 && pointroll < 90) *pointvalue = 100;
            	if(pointroll >= 90) *pointvalue = 200;
    	}
}

int is_p1_collision(int *p1x, int *p1y, int playerX, int playerY, int *ep1, long time, long *t_init, int *ex, int *ey, struct firefly all[500], double *score, int *fireflies_k) {
    	double xdistance = abs(*p1x - playerX);
    	double ydistance = abs(*p1y - playerY);
    	double distance = sqrt(pow(xdistance,2) + pow(ydistance,2));
    	if(distance < 20) {
            	*ex = *p1x;
            	*ey = *p1y;
            	clear_fireflies(all, *p1x, *p1y, score, fireflies_k);
            	*p1x = 0;
            	*p1y = 0;
            	*ep1 = 0;
            	*t_init = time;
    	}
}

int is_p2_collision(int *p2x, int *p2y, int playerX, int playerY, int *ep2, int *pointvalue, double *score) {
    	double xdistance = abs(*p2x - playerX);
    	double ydistance = abs(*p2y - playerY);
    	double distance = sqrt(pow(xdistance,2) + pow(ydistance,2));
    	if(distance < 15) {
            	*p2x = 0;
            	*p2y = 0;
            	*ep2 = 0;
            	*score += *pointvalue;
            	*pointvalue = 0;
    	}
}

void draw_explosion(int xc, int yc, long t_init, long t_current) {
    	gfx_color(255,125,0);
    	int dt = abs(t_current - t_init);
    	if(dt < 375000/3) draw_circle(xc,yc,10);
    	if(dt < 750000/3) draw_circle(xc,yc,20);
    	if(dt < 875000/3) draw_circle(xc,yc,30);
    	if(dt < 1000000/3) draw_circle(xc,yc,40);
    	if(dt < 1125000/3) draw_circle(xc,yc,50);
    	if(dt < 1250000/3) draw_circle(xc,yc,60);
    	if(dt > 125000/3 && dt < 1375000/3) draw_circle(xc,yc,70);
    	if(dt > 250000/3 && dt < 1500000/3) draw_circle(xc,yc,80);
    	if(dt > 375000/3 && dt < 1625000/3) draw_circle(xc,yc,90);
    	if(dt > 500000/3 && dt < 1750000/3) draw_circle(xc,yc,100);
}

void draw_numeral(int number, int x, int y, int width, int height) {
    	switch(number) {
            	case 0:
                    	draw_zero(x,y,width,height);
                    	break;
            	case 1:
                    	draw_one(x,y,width,height);
                    	break;
            	case 2:
                    	draw_two(x,y,width,height);
                    	break;
            	case 3:
                    	draw_three(x,y,width,height);
                    	break;
            	case 4:
                    	draw_four(x,y,width,height);
                    	break;
            	case 5:
                    	draw_five(x,y,width,height);
                    	break;
            	case 6:
                    	draw_six(x,y,width,height);
                    	break;
            	case 7:
                    	draw_seven(x,y,width,height);
                    	break;
            	case 8:
                    	draw_eight(x,y,width,height);
                    	break;
            	case 9:
                    	draw_nine(x,y,width,height);
                    	break;
    	}
}

void draw_zero(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y, x, y + height);
    	gfx_line(x + width, y, x + width, y + height);
    	gfx_line(x, y + height, x + width, y + height);
}

void draw_one(int x, int y, int width, int height) {
    	gfx_line(x + width, y, x + width, y + height);
}

void draw_two(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height/2, x + width, y + height/2);
    	gfx_line(x, y + height, x + width, y + height);
    	gfx_line(x + width, y, x + width, y + height/2);
    	gfx_line(x, y + height/2, x, y + height);
}

void draw_three(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height/2, x + width, y + height/2);
    	gfx_line(x, y + height, x + width, y + height);
    	gfx_line(x + width, y, x + width, y + height);
}

void draw_four(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height/2);
    	gfx_line(x, y + height/2, x + width, y + height/2);
    	gfx_line(x + width, y, x + width, y + height);
}

void draw_five(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height/2, x + width, y + height/2);
    	gfx_line(x, y + height, x + width, y + height);
    	gfx_line(x, y, x, y + height/2);
    	gfx_line(x + width, y + height/2, x + width, y + height);
}

void draw_six(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height/2, x + width, y + height/2);
    	gfx_line(x, y + height, x + width, y + height);
    	gfx_line(x, y, x, y + height);
    	gfx_line(x + width, y + height/2, x + width, y + height);
}

void draw_seven(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x + width, y, x + width, y + height);
}

void draw_eight(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height/2, x + width, y + height/2);
    	gfx_line(x, y + height, x + width, y + height);
    	gfx_line(x, y, x, y + height);
    	gfx_line(x + width, y, x + width, y + height);
}

void draw_nine(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height/2, x + width, y + height/2);
    	gfx_line(x, y + height, x + width, y + height);
    	gfx_line(x + width, y, x + width, y + height);
    	gfx_line(x, y, x, y + height/2);
}

void draw_firefly(int x, int y) {
    	gfx_color(255,255,0);
    	gfx_line(x+1,y,x-1,y);
    	gfx_line(x,y+1,x,y-1);
}

void draw_circle(double xc, double yc, double r) {
    	double xlast = xc + r;
    	double ylast = yc;
    	double x,y;
    	double i;
    	double d = 0.1;
    	for(i = 0; i < 2 * M_PI + d; i += d) {
            	x = xc + r * cos(i);
            	y = yc + r * sin(i);
            	gfx_line(x,y,xlast,ylast);
            	xlast = x;
            	ylast = y;
    	}
}

void draw_p1(double xc, double yc, double r1, double r2, double phase) {
    	double r3 = r1 - 0.1 * r1;
    	double r4 = r2 + 0.1 * r1;
    	double i = 0;
    	double angle = 3 * M_PI / 2 + phase;
    	gfx_color(255,255,0);
    	draw_circle(xc,yc,r1);
    	draw_circle(xc,yc,r2);
    	draw_arc(xc, yc, angle - M_PI / 8, angle + M_PI / 8, r3);
    	draw_arc(xc, yc, angle - M_PI / 8, angle + M_PI / 8, r4);
    	draw_arc(xc, yc, angle + 2 * M_PI / 3 - M_PI / 8, angle + 2 * M_PI / 3 + M_PI / 8, r3);
    	draw_arc(xc, yc, angle + 2 * M_PI / 3 - M_PI / 8, angle + 2 * M_PI / 3 + M_PI / 8, r4);
    	draw_arc(xc, yc, angle - 2 * M_PI / 3 - M_PI / 8, angle - 2 * M_PI / 3 + M_PI / 8, r3);
    	draw_arc(xc, yc, angle - 2 * M_PI / 3 - M_PI / 8, angle - 2 * M_PI / 3 + M_PI / 8, r4);

    	gfx_line(xc + r3 * cos(angle - M_PI / 8), yc + r3 * sin(angle - M_PI / 8), xc + r4 * cos(angle - M_PI / 8), yc + r4 * sin(angle - M_PI / 8));
    	gfx_line(xc + r3 * cos(angle + M_PI / 8), yc + r3 * sin(angle + M_PI / 8), xc + r4 * cos(angle + M_PI / 8), yc + r4 * sin(angle + M_PI / 8));
    	gfx_line(xc + r3 * cos(angle + 2 * M_PI / 3 - M_PI / 8), yc + r3 * sin(angle + 2 * M_PI / 3 - M_PI / 8), xc + r4 * cos(angle + 2 * M_PI / 3 - M_PI / 8), yc + r4 * sin(angle + 2 * M_PI / 3 - M_PI / 8));
    	gfx_line(xc + r3 * cos(angle + 2 * M_PI / 3 + M_PI / 8), yc + r3 * sin(angle + 2 * M_PI / 3 + M_PI / 8), xc + r4 * cos(angle + 2 * M_PI / 3 + M_PI / 8), yc + r4 * sin(angle + 2 * M_PI / 3 + M_PI / 8));
    	gfx_line(xc + r3 * cos(angle - 2 * M_PI / 3 - M_PI / 8), yc + r3 * sin(angle - 2 * M_PI / 3 - M_PI / 8), xc + r4 * cos(angle - 2 * M_PI / 3 - M_PI / 8), yc + r4 * sin(angle - 2 * M_PI / 3 - M_PI / 8));
    	gfx_line(xc + r3 * cos(angle - 2 * M_PI / 3 + M_PI / 8), yc + r3 * sin(angle - 2 * M_PI / 3 + M_PI / 8), xc + r4 * cos(angle - 2 * M_PI / 3 + M_PI / 8), yc + r4 * sin(angle - 2 * M_PI / 3 + M_PI / 8));
}

void draw_p2(double xc, double yc, double phase, double pointvalue) {
    	double r = 15;
    	double x1 = xc + r;
    	double x2 = xc + r/2;
    	double x3 = xc - r/2;
    	double x4 = xc - r;
    	double y1 = yc + r;
    	double y2 = yc + r/2;
    	double y3 = yc - r/2;
    	double y4 = yc - r;

    	if(pointvalue == 25) {
            	gfx_color(0,255,255);
    	} else if(pointvalue == 50) {
            	gfx_color(0,255,0);
    	} else if(pointvalue == 100) {
            	gfx_color(255,255,0);
    	} else if(pointvalue == 200) {
            	gfx_color(255,80,0);
    	}

    	gfx_line(x1 + 3 * cos(phase), yc, x2 - 2.12 * cos(phase), y2 + 2.12 * cos(phase));
    	gfx_line(x1 + 3 * cos(phase), yc, x2 - 2.12 * cos(phase), y3 - 2.12 * cos(phase));
    	gfx_line(x4 - 3 * cos(phase), yc, x3 + 2.12 * cos(phase), y2 + 2.12 * cos(phase));
    	gfx_line(x4 - 3 * cos(phase), yc, x3 + 2.12 * cos(phase), y3 - 2.12 * cos(phase));
    	gfx_line(xc, y1 + 3 * cos(phase), x2 - 2.12 * cos(phase), y3 - 2.12 * cos(phase));
    	gfx_line(xc, y1 + 3 * cos(phase), x3 + 2.12 * cos(phase), y3 - 2.12 * cos(phase));
    	gfx_line(xc, y4 - 3 * cos(phase), x2 - 2.12 * cos(phase), y2 + 2.12 * cos(phase));
    	gfx_line(xc, y4 - 3 * cos(phase), x3 + 2.12 * cos(phase), y2 + 2.12 * cos(phase));
}

void draw_arc(double xc, double yc, double theta, double phi, double r) {
    	double xi = xc + r * cos(theta);
    	double yi = yc + r * sin(theta);
    	double xf,yf;
    	double i;
    	for(i = theta; i <= phi + 0.1; i += 0.1) {
            	xf = xc + r * cos(i);
            	yf = yc + r * sin(i);
            	gfx_line(xi,yi,xf,yf);
            	xi = xf;
            	yi = yf;
    	}
}

void draw_A(int x, int y, int width, int height) {
    	gfx_line(x + width / 2, y, x, y + height);
    	gfx_line(x + width / 2, y, x + width, y + height);
    	gfx_line(x + width / 4, y + height / 2, x + 3 * width / 4, y + height / 2);
}

void draw_C(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y + height, x + width, y + height);
}

void draw_D(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y, x + width, y + height / 2);
    	gfx_line(x, y + height, x + width, y + height / 2);
}

void draw_E(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x ,y, x + width, y);
    	gfx_line(x, y + height / 2, x + width, y + height / 2);
    	gfx_line(x, y + height, x + width, y + height);
}

void draw_F(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height / 2, x + width / 2, y + height / 2);
}

void draw_G(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y + height, x + width, y + height);
    	gfx_line(x + width, y + height, x + width, y + height / 2);
    	gfx_line(x + width, y + height / 2, x + width / 2, y + height / 2);
    	gfx_line(x, y, x + width, y);
}

void draw_H(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x + width, y, x + width, y + height);
    	gfx_line(x, y + height / 2, x + width, y + height / 2);
}

void draw_I(int x, int y, int width, int height) {
    	gfx_line(x + width / 2, y, x + width / 2, y + height);
}

void draw_L(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y + height, x + width, y + height);
}

void draw_M(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y, x + width / 2, y + height / 2);
    	gfx_line(x + width/ 2, y + height / 2, x + width, y);
    	gfx_line(x + width, y, x + width, y + height);
}

void draw_N(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y, x + width, y + height);
    	gfx_line(x + width, y, x + width, y + height);
}

void draw_O(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height, x + width, y + height);
    	gfx_line(x + width, y, x + width, y + height);
}

void draw_P(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height / 2, x + width, y + height / 2);
    	gfx_line(x + width, y, x + width, y + height / 2);
}

void draw_R(int x, int y, int width, int height) {
    	gfx_line(x, y, x, y + height);
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y + height / 2, x + width, y + height / 2);
    	gfx_line(x + width, y, x + width, y + height / 2);
    	gfx_line(x, y + height / 2, x + width, y + height);
}

void draw_S(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x, y, x, y + height / 2);
    	gfx_line(x, y + height / 2, x + width, y + height / 2);
    	gfx_line(x + width, y + height / 2, x + width, y + height);
    	gfx_line(x, y + height, x + width, y + height);
}

void draw_T(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width, y);
    	gfx_line(x + width / 2, y, x + width / 2, y + height);
}

void draw_V(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width / 2, y + height);
    	gfx_line(x + width / 2, y + height, x + width, y);
}

void draw_Y(int x, int y, int width, int height) {
    	gfx_line(x, y, x + width / 2, y + height / 2);
    	gfx_line(x + width / 2, y + height / 2, x + width, y);
    	gfx_line(x + width / 2, y + height / 2, x + width / 2, y + height);
}

void draw_slash(int x, int y, int width, int height) {
    	gfx_line(x, y + height, x + width, y);
}

void draw_plus(int x, int y, int width, int height) {
    	gfx_line(x + width / 2, y + height / 4, x + width / 2, y + 3 * height / 4);
    	gfx_line(x, y + height / 2, x + width, y + height / 2);
}

void draw_string(char *s, int xc, int yc, int width, int height) {
    	gfx_color(255,0,0);
    	int i;
    	int current_char;
    	for(i = 0; s[i]; i++) {
            	xc += width + 5;
            	current_char = toupper(s[i]);
            	switch(current_char) {
                    	case 'A':
                            	draw_A(xc,yc,width,height);
                            	break;
                    	case 'B':
                            	draw_eight(xc,yc,width,height);
                            	break;
                    	case 'C':
                            	draw_C(xc,yc,width,height);
                            	break;
                    	case 'D':
                            	draw_D(xc,yc,width,height);
                            	break;
                    	case 'E':
                            	draw_E(xc,yc,width,height);
                            	break;
                    	case 'F':
                            	draw_F(xc,yc,width,height);
                            	break;

                    	case 'G':
                            	draw_G(xc,yc,width,height);
                            	break;
                    	case 'H':
                            	draw_H(xc,yc,width,height);
                            	break;
                    	case 'I':
                            	draw_I(xc,yc,width,height);
                            	break;
                    	case 'L':
                            	draw_L(xc,yc,width,height);
                            	break;
                    	case 'M':
                            	draw_M(xc,yc,width,height);
                            	break;
                    	case 'N':
                            	draw_N(xc,yc,width,height);
                            	break;
                    	case 'O':                            	draw_O(xc,yc,width,height);
                            	break;
                    	case 'P':
                            	draw_P(xc,yc,width,height);
                            	break;
                    	case 'R':
                            	draw_R(xc,yc,width,height);
                            	break;
                    	case 'S':
                            	draw_S(xc,yc,width,height);
                            	break;
                    	case 'T':
                            	draw_T(xc,yc,width,height);
                            	break;
                    	case 'V':
                            	draw_V(xc,yc,width,height);
                            	break;
                    	case 'Y':
                            	draw_Y(xc,yc,width,height);

                            	break;
                    	case '/':
                            	draw_slash(xc,yc,width,height);
                            	break;
                    	default:
                            	xc += width;
                            	break;
            	}
    	}
}

void draw_ship(double direction, double xc, double yc) {
    	gfx_color(0,255,0);
    	direction = 2 * M_PI - direction;
    	double x1 = xc + 5 * cos(direction);
    	double y1 = yc + 5 * sin(direction);
    	double x2 = xc + 5 * cos(direction + M_PI);
    	double y2 = yc + 5 * sin(direction + M_PI);
    	double x3 = xc + 8 * cos(direction + 5 * M_PI / 6);
    	double y3 = yc + 8 * sin(direction + 5 * M_PI / 6);
    	double x4 = xc + 8 * cos(direction + 7 * M_PI / 6);
    	double y4 = yc + 8 * sin(direction + 7 * M_PI / 6);
    	gfx_line(x1,y1,x3,y3);
    	gfx_line(x1,y1,x4,y4);
    	gfx_line(x2,y2,x3,y3);
    	gfx_line(x2,y2,x4,y4);
}

double process_keys(char input) {
    	double direction;
    	if(input == 'w') direction = M_PI / 2;
    	if(input == 'a') direction = M_PI;
    	if(input == 's') direction = 3 * M_PI / 2;
    	if(input == 'd') direction = 0;
    	return direction;
}

void move_player(double dt, double direction, double *xp, double *yp, double vmax) {
    	double vx = vmax * cos(direction);
    	double vy = vmax * sin(direction);
    	*xp += vx * (dt/1000000);
    	*yp -= vy * (dt/1000000);
}

void move_firefly(struct firefly all[500],struct firefly *one, double xp, double yp, double dt){

    	double y = yp - one->ypos;
    	double x = xp - one->xpos;
    	double direction = atan2(y,x);
    	double vx = one->v * cos(direction);
    	double vy = one->v * sin(direction);
    	double nx = one->xpos+vx*(dt/1000000);
    	double ny = one->ypos+vy*(dt/1000000);
    	one->xpos = nx;
    	one->ypos = ny;
    	draw_firefly(one->xpos, one->ypos);
}

void add_fireflies(struct firefly all[500], double xp, double yp){
    	int r = rand()%1000;
    	int i=0, x, y;
    	if (r > 983){
            	while(all[i].v){
                    	i++;
            	}
            	if(i<500){
                    	x = rand()%500;
                    	y = rand()%500;
                    	if ( x > xp+90 || x < xp-90 ){
                            	all[i].xpos = x;
                    	} else{
                            	all[i].xpos = rand()%500;
                    	}
                    	if ( y > yp+90 || y < yp-90){
                            	all[i].ypos = y;
                    	} else{

                            	all[i].ypos = rand()%500;
                    	}
                    	all[i].v = rand()%35+15;
            	}
    	}
}

void move_allfireflies(struct firefly all[500], double xp, double yp, double dt){
    	int i=0;
    	while(all[i].v && i < 500){
            	move_firefly(all, &all[i], xp, yp, dt);
            	i++;
    	}
}



int check_line(double x1, double y1, double x2, double y2, double x, double y){
    	double my1 = y-y1;
    	double my2 = y-y2;
    	double mx1 = x-x1;
    	double mx2 = x-x2;
    	double length = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
    	if(sqrt(mx1*mx1 + my1*my1) <= length+1&& sqrt(mx2*mx2 + my2*my2) <= length+1){
            	if( fabs(my1/mx1 - my2/mx2) < .2 ){
                    	return 1;
            	}
    	}
    	if(round(x1)==round(x) && round(y1)== round(y)){
            	return 1;
    	} else if(round(x2)==round(x) && round(y2)==round(y)){
            	return 1;
    	}
    	return 0;
}

int check_fcollision(struct firefly all[500], double xc, double yc, double direction){
direction = 2 * M_PI - direction;
    	double x1 = xc + 5 * cos(direction);
    	double y1 = yc + 5 * sin(direction);
    	double x2 = xc + 5 * cos(direction + M_PI);
    	double y2 = yc + 5 * sin(direction + M_PI);
    	double x3 = xc + 8 * cos(direction + 5 * M_PI / 6);
    	double y3 = yc + 8 * sin(direction + 5 * M_PI / 6);
    	double x4 = xc + 8 * cos(direction + 7 * M_PI / 6);
    	double y4 = yc + 8 * sin(direction + 7 * M_PI / 6);

	int i=0, c=0;
	while(all[i].v && i<500){
c+=check_line(x1, y1, x3, y3, all[i].xpos, all[i].ypos);
	c+=check_line(x1, y1, x4, y4, all[i].xpos, all[i].ypos);
	c+=check_line(x2, y2, x3, y3, all[i].xpos, all[i].ypos);
	c+=check_line(x2, y2, x4, y4, all[i].xpos, all[i].ypos);
	i++;
}
if (c==0){
	return 0;
}else{
	return 1;
}
}

void shift_array(struct firefly all[500], int i){
    	for(i = i; i<500-i; i++){
            	if(all[i].v){
                    	all[i].xpos = all[i+1].xpos;
                    	all[i].ypos = all[i+1].ypos;
                    	all[i].v = all[i+1].v;
            	}
    	}
}

void clear_fireflies(struct firefly all[500], double xp, double yp, double *score, int *fireflies_k){
    	int i=0;
    	double x, y;
    	while(all[i].v){
            	x = all[i].xpos - xp;
            	y = all[i].ypos - yp;
            	if (sqrt(x*x + y*y) < 100){
                    	*score += 10;
                    	*fireflies_k++;
                    	shift_array(all, i);
                    	i--;
            	}
            	i++;
    	}
}



