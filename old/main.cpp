#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include<io.h>
#include<fcntl.h>
#include<conio.h>

#ifndef WIN32
#include <sys/time.h>
#endif
/*
#ifdef WIN32
#include <GL/glew.h>
#endif
*/
#include <GL/freeglut.h>

#include"al.h"
#include"alc.h"

#include "common.h"
#include "gettimeofday.h"
#include "render.h"
#include "texture.h"
#include "math.h"
#include "list.h"

#include "entity.h"

int hMainWindow; // Main Window

// keyboard
bool global_keys[256];
bool global_keysonce[256];
bool global_specialkeys[256];

// mouse
int global_mouse[3];
int global_mousepos[2];

Render *render;
Texture *texture;

struct timeval tick_otime, tick_ntime, tick_diff;

AnimatedEntity *Player;
List<Entity> *Bullets;
List<AnimatedEntity> *Zombies;

float overall_gameTime;
float zombie_lastSpawnTime;
float gameover_time;

int mode_current;
int mode_next;

byte guiCharSize[256];

void exitGame() {
	exit(0);
}

void switchMode(const int &mode) {
	if (mode != 1) {
		Zombies->ClearAndDelete();
		Bullets->ClearAndDelete();

		if (mode == 2)
			gameover_time = 0.0f;

		//glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	} else {
		Player->GetPos()->Set(1024.0f / 2, 768.0f / 2);
		Player->GetOldPos()->Set(1024.0f / 2, 768.0f / 2);
		Player->SetAngle(0.0f);
		Player->SetSpeed(100.0f);

		Zombies->ClearAndDelete();
		Bullets->ClearAndDelete();
		overall_gameTime = 0.0f;
		zombie_lastSpawnTime = 0.0f;
//		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	}
	mode_current = mode;
}

// KeyDown Event
static void key(unsigned char k, int x, int y) {
	global_keys[k] = true;
	global_keysonce[k] = true;
}

// KeyUp Event
static void keyup(unsigned char k, int x, int y) {
	global_keys[k] = false;
}

// Special KeyDown Event
static void specialkey(int key, int x, int y) {
	global_specialkeys[key] = true;
}

// Special KeyUp Event
static void specialkeyup(int key, int x, int y) {
	global_specialkeys[key] = false;
}

// Mouse Button Event
static void mousebutton(int button, int state, int x, int y) {
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				global_mouse[0] = 1;
			}
			else if (state == GLUT_UP) {
				global_mouse[0] = 2;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
				global_mouse[1] = 1;
			else if (state == GLUT_UP)
				global_mouse[1] = 2;
			break;
		default:
			break;
	}
	global_mousepos[0] = x;
	global_mousepos[1] = y;
}

// Mouse Move Event
static void mousemotion(int x, int y) {
	global_mousepos[0] = x;
	global_mousepos[1] = y;
}

// Idle function
static void idle(void) {
	float overall_msecs;
	float local_mousepos[2];

	// QUIT GAME
	if (global_keysonce[27]) {
		if (mode_current == 1) {
			mode_next = 0;
		} else {
			exitGame();
		}
	}

	if (mode_current != mode_next)
		switchMode(mode_next);

	gettimeofday(&tick_ntime, NULL);
	getdifftime(tick_ntime, tick_otime, &tick_diff);
	tick_otime = tick_ntime;

	overall_msecs = tick_diff.tv_sec * 1000 + tick_diff.tv_usec / 1000.0f;
	// slow time!
	if (overall_msecs > 20.0f)
		overall_msecs = 20.0f;

//	local_mousepos[0] = (float)global_mousepos[0];
//	local_mousepos[1] = 767.0f - global_mousepos[1];
	local_mousepos[0] = (int)(global_mousepos[0] * 1024 / render->GetScreenWidth());
	local_mousepos[1] = 767 - (int)(global_mousepos[1] * 768 / render->GetScreenHeight());


	if (mode_current == 0) { // menu
		if (global_mouse[0] == 2) {
			if (local_mousepos[0] >= 1024.0f/2.0f - 64.0f && local_mousepos[0] < 1024.0f/2.0f + 64.0f) {
				if (local_mousepos[1] >= 768.0f/2.0f + 8.0f && local_mousepos[1] < 768.0f/2.0f + 72.0f) {
					mode_next = 1;
				} else if (local_mousepos[1] >= 768.0f/2.0f - 72.0f && local_mousepos[1] < 768.0f/2.0f - 8.0f) {
					exitGame();
				}
			}
		}
	} else if (mode_current == 1) { // game
		Vector player_move;
		Vector pos;
		float cur_move;
		static ListReader<AnimatedEntity> *zombiesReader = new ListReader<AnimatedEntity>();
		static ListReader<Entity> *bulletsReader = new ListReader<Entity>();

		// Player
		{
			player_move.Set(0.0f, 0.0f);
			cur_move = Player->GetSpeed() * overall_msecs / 1000.0f;

			if (global_keys['w'])
				player_move.SetY(1.0f);
			if (global_keys['s'])
				player_move.SetY(-1.0f);
			if (global_keys['a'])
				player_move.SetX(-1.0f);
			if (global_keys['d'])
				player_move.SetX(1.0f);

			player_move.Normalize();
			player_move.Scale(cur_move);
		
			Player->GetPos()->Add(player_move);
		
			if (Player->GetPos()->GetX() < 0)
				Player->GetPos()->SetX(0.0f);
			else if (Player->GetPos()->GetX() > 1023.0f)
				Player->GetPos()->SetX(1023.0f);
		
			if (Player->GetPos()->GetY() < 0)
				Player->GetPos()->SetY(0.0f);
			if (Player->GetPos()->GetY() > 767.0f)
				Player->GetPos()->SetY(767.0f);

			// Angle calculation
			{
				Vector v;

				v.Set(local_mousepos[0], local_mousepos[1]);
				Player->SetAngle(Math::CalculateAngle(*Player->GetPos(), v));
			}
		}

		// Zombies
		{
			AnimatedEntity *zombie;

			// new zombies
			if (zombie_lastSpawnTime + 500.f < overall_gameTime) {
				if (rand() % 4 == 0) {
					int newZombies;
					int i;

					newZombies = (int)(overall_gameTime / 5000.f);
					if (newZombies <= 0)
						newZombies = 2;

					newZombies = rand() % newZombies;
					for(i = 0; i < newZombies; i++) {
						int pos;
						
						zombie = new AnimatedEntity();
						zombie->SetAngle(0.0f);
						zombie->SetSpeed(50.0f);

						pos = rand() % 4;
						if (pos == 0)
							zombie->GetPos()->Set(0.0f, (float)(rand() % 768));
						else if (pos == 1)
							zombie->GetPos()->Set((float)(rand() % 1024), 767.0f);
						else if (pos == 2)
							zombie->GetPos()->Set(1023.0f, (float)(rand() % 768));
						else if (pos == 3)
							zombie->GetPos()->Set((float)(rand() % 1024), 0.0f);
						
						Zombies->AddElement(zombie);
					}
					zombie_lastSpawnTime = overall_gameTime;
				}

			}

			zombiesReader->Attach(Zombies);

			zombie = zombiesReader->GoFirstRef();
			while(zombie) {
				float animTime;
				Vector v;

				animTime = zombie->GetAnimationTime();
				animTime += overall_msecs;
				if (animTime >= 600.0f)
					animTime -= 600.0f;
				zombie->SetAnimationTime(animTime);

				zombie->SetAngle(Math::CalculateAngle(*zombie->GetPos(), *Player->GetPos()));
				zombie->GetOldPos()->Set(*zombie->GetPos());

				cur_move = zombie->GetSpeed() * overall_msecs / 1000.0f;
				Math::CalculateVector(zombie->GetAngle(), v);
				v.Scale(cur_move);
				zombie->GetPos()->Add(v);

				// check if the zombie hits our player
				v.Sub(*zombie->GetPos(), *Player->GetPos());
				if (v.Length() <= 22.0f) {
					printf("GAME OVER\n");
					printf("You've survived %.3f\n", overall_gameTime / 1000.f);
					mode_next = 2;
					break;
				}

				zombie = zombiesReader->GoNextRef();
			}
		}

		// Bullets
		if (mode_next != 2) {
			float animTime;
			Entity *bullet;
			AnimatedEntity *zombie;

			animTime = Player->GetAnimationTime();
			if (animTime > 0.0f) {
				animTime -= overall_msecs;
				if (animTime < 0.0f)
					animTime = 0.0f;
				Player->SetAnimationTime(animTime);
			}

			// new bullet
			if (global_mouse[0] == 1 && Player->GetAnimationTime() <= 0.0f) {
				Player->SetAnimationTime(200.0f);

				bullet = new Entity();
				bullet->GetPos()->Set(*Player->GetPos());
				bullet->GetOldPos()->Set(*Player->GetPos());
				bullet->SetAngle(Player->GetAngle());
				bullet->SetSpeed(2000.f);

				Bullets->AddElement(bullet);
			}

			bulletsReader->Attach(Bullets);
			bullet = bulletsReader->GoFirstRef();
			while(bullet) {
				Vector v;
				float bestHitDistance = -1.0f;
				AnimatedEntity *bestZombie = NULL;

				cur_move = bullet->GetSpeed() * overall_msecs / 1000.0f;
				bullet->GetOldPos()->Set(*bullet->GetPos());
				Math::CalculateVector(bullet->GetAngle(), v);

				// zombie hit???
				zombiesReader->Attach(Zombies);
				zombie = zombiesReader->GoFirstRef();
				while(zombie) {
					int ok = 0;
					float hitDistance;
					Vector diff;

					diff.Sub(*zombie->GetPos(), *bullet->GetPos());
					if (diff.Length() > 11.0f) {
						hitDistance = Math::CalculateRayCircleIntersectionDistance(*bullet->GetPos(), v, *zombie->GetPos(), 11.0f);
						if (hitDistance >= 0.0f && hitDistance <= cur_move)
							ok = 1;
					} else {
						ok = 1;
						hitDistance = 0.0f;
					}

					if (ok) {
						if (bestHitDistance < 0.0f || hitDistance < bestHitDistance) {
							bestHitDistance = hitDistance;
							bestZombie = zombie;
						}
					}
					zombie = zombiesReader->GoNextRef();
				}

				if (bestZombie) {
					Zombies->RemoveElement(bestZombie);
					delete bestZombie;

					Bullets->RemoveElement(bullet);
					delete bullet;
					bullet = NULL;
				}

				if (bullet) {
					v.Scale(cur_move);
					bullet->GetPos()->Add(v);

					if (bullet->GetPos()->GetX() < 0.0f || bullet->GetPos()->GetX() > 1023.0f || bullet->GetPos()->GetY() < 0.0f || bullet->GetPos()->GetY() > 767.0f) {
						Bullets->RemoveElement(bullet);
						delete bullet;
						bullet = NULL;
					}
				}

				bullet = bulletsReader->GoNextRef();
			}

		}

		overall_gameTime += overall_msecs;
	} else if (mode_current == 2) { // game over
		gameover_time += overall_msecs;
		if (gameover_time >= 5000.0f)
			mode_next = 0;
	}

	if (global_mouse[0] == 2)
		global_mouse[0] = 0;
	if (global_mouse[1] == 2)
		global_mouse[1] = 0;

	memset(global_keysonce, 0, sizeof(bool)* 256);

	// render
	glutPostRedisplay();
}

void drawString(const float &x, const float &y, const char *str, const float size) {
	char *c;
	float px, py;
	float cell;

/*
	texture->Bind(texture->GetFont());

	cell = 32.0f / 512.0f;
	
	c = (char *)str;
	px = x + size / 2.0f;
	py = y - size / 2.0f;
	while(*c) {
		render->DrawSprite(px, py, size, size, (*c % 16) * cell, floor(*c / 16.0f) * cell, cell, cell, 0.0f);
		px += (float)guiCharSize[*c] * size / 32.0f;
		c++;
	}
*/
	texture->Bind(texture->GetFontFixed());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	cell = 16.0f / 512.0f;
	c = (char *)str;
	px = x + size / 2.0f;
	py = y - size / 2.0f;
	
	while(*c) {
		render->DrawSprite(px, py, size, size, (*c % 32) * cell, floor(*c / 32.0f) * cell, cell, cell, 0.0f);
		px += 16.0f * (size / 16.0f);
		c++;
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Rendering
void display(void) {
	Vector pos;
	float cell;

	render->Begin();
	render->SetTextureMode();
	render->Set2DMode(1024, 768);

	cell = 32.0f / 256.0f;

	if (mode_current == 0) { // menu
		texture->Bind(texture->GetGFX());

		render->DrawSprite(1024.0f / 2.0f, 768.0f / 2.0f + 40.0f, 128.0f, 64.0f, 0.0f * cell, 2.0f * cell, 2.0f * cell, 1.0f * cell, 0.0f);
		render->DrawSprite(1024.0f / 2.0f, 768.0f / 2.0f - 40.0f, 128.0f, 64.0f, 2.0f * cell, 2.0f * cell, 2.0f * cell, 1.0f * cell, 0.0f);
	} else if (mode_current == 1) { // game
		// background
		texture->Bind(0);

		glColor3f(0.0f, 0.3f, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 768.0f);
		glVertex2f(1024.0f, 768.0f);
		glVertex2f(1024.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
		texture->Bind(texture->GetGFX());

		// player
		{
			float animCell;

			if (Player->GetAnimationTime() > 100.0f)
				animCell = 1.0f;
			else if (Player->GetAnimationTime() > 0.0f)
				animCell = 2.0f;
			else
				animCell = 0.0f;

			render->DrawSprite(Player->GetPos()->GetX(), Player->GetPos()->GetY(), 64.0f, 64.0f, animCell * cell, 0.0f, cell, cell, Player->GetAngle());
		}

		// zombies
		{
			AnimatedEntity *zombie;
			static ListReader<AnimatedEntity> *zombiesReader = new ListReader<AnimatedEntity>();
			float animCell;

			zombiesReader->Attach(Zombies);
			zombie = zombiesReader->GoFirstRef();
			while(zombie) {
				animCell = floor(zombie->GetAnimationTime() / 100.0f);

				render->DrawSprite(zombie->GetPos()->GetX(), zombie->GetPos()->GetY(), 64.0f, 64.0f, animCell * cell, 1.0f * cell, cell, cell, zombie->GetAngle());

				zombie = zombiesReader->GoNextRef();
			}
		}

		// bullets
		{
			Entity *bullet;
			static ListReader<Entity> *bulletsReader = new ListReader<Entity>();

			bulletsReader->Attach(Bullets);
			bullet = bulletsReader->GoFirstRef();
			while(bullet) {
				render->DrawSprite(bullet->GetPos()->GetX(), bullet->GetPos()->GetY(), 64.0f, 64.0f, 3.0f * cell, 0.0f, cell, cell, bullet->GetAngle());

				bullet = bulletsReader->GoNextRef();
			}
		}
	} else if (mode_current == 2) { // game over
		texture->Bind(texture->GetGFX());
		render->DrawSprite(1024.0f / 2.0f, 768.0f / 2.0f, 256.0f, 64.0f, 4.0f * cell, 2.0f * cell, 4.0f * cell, 1.0f * cell, 0.0f);
	}

	{
		if (mode_current == 1) {
			char buf[256];

			sprintf(buf, "Time: %.0f", overall_gameTime / 1000.f);
			drawString(0.0f, 768.0f, buf, 16.0f);
		}
	}

	{
		float local_mousepos[2];

		texture->Bind(texture->GetGFX());

		local_mousepos[0] = global_mousepos[0] * 1024.0f / render->GetScreenWidth();
		local_mousepos[1] = 767.0f - global_mousepos[1] * 768.0f / render->GetScreenHeight();
		render->DrawSprite(local_mousepos[0], local_mousepos[1], 16.0f, 16.0f, 0.0f, 3.0f * cell, cell / 2.0f, cell / 2.0f, 0.0f);
	}

	render->End2DMode();
	render->End();
}

void reshape(int w, int h) {
	render->Reshape(w, h);
}

// Entry point
int main(int argc, char** argv) {
	int i;

	srand(time(NULL));

	// clear
	for (i = 0; i<256; i++) {
		global_keys[i] = false;
		global_specialkeys[i] = false;
	}
	for (i = 0; i<3; i++)
		global_mouse[i] = false;

	// Screen Init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(10, 10);

	glutCreateWindow("Fast Shooter");

	render = new Render();
	render->Init(1024, 768);

	texture = new Texture();
	texture->LoadTexturesList();

	// Functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyup);
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemotion);
	glutPassiveMotionFunc(mousemotion);
	glutSpecialFunc(specialkey);
	glutSpecialUpFunc(specialkeyup);
	glutIdleFunc(idle);

	//glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	glutSetCursor(GLUT_CURSOR_NONE);

	Player = new AnimatedEntity();
	Zombies = new List<AnimatedEntity>();
	Bullets = new List<Entity>();

	mode_current = 0;
	{
		FILE *fp;
		char buf[256];

		fp = fopen("chars.cfg", "r");
		if (fp) {
			int i;

			for (i=0;i<256;i++) {
				fgets(buf, 250, fp);
				guiCharSize[i] = (byte)atoi(buf) * 2;
			}
			fclose(fp);
		}
	}

	glutMainLoop();
	return 0;
}
