/****************************************************************************
*                                                                           *
*    This program is distributed in the hope that it will be useful,        *
*    but without any warranty, without even the implied warranty of         *
*    particular purpose.                                                    *
*                                                                           *
*	  Nova32 Development Software                                           *
*	  (c) Rolando Fernandez Benavidez, March 2019.                          *
*                                                                           *
*****************************************************************************
*                       A S T E R O I D S - T E R R A                       *
*                                 use only                                  *
*                           GNU GCC-COFF & Jo-Engine                        *
*                                                                           *
* Platform: SEGA SATURN                                                     *
*                                                                           *
* By Rolando Fernandez Benavidez                                            *
* Version 3.0.0                                                             *
****************************************************************************/

#ifndef __GLOBAL_H__
# define __GLOBAL_H__

#define ASTEROIDS_VERSION 3

#define MAXMAP 10
#define TYPE
#define SCREEN_V_MIN   0
#define SCREEN_V_MAX   200
#define SCREEN_H_MIN   0
#define SCREEN_H_MAX   280

#define CD_LOOP         1
#define TRACK_INTRO     2
#define TRACK_LEVEL1    3
#define TRACK_LEVEL2    4
#define TRACK_LEVEL3    5
#define TRACK_LEVEL4    6
#define TRACK_LEVEL5    7

#define RUN             if(Run){
#define END_RUN         }

#define DEBUG_GO        if(Debug_Show == FALSE){
#define DEBUG_END       }

#define SPEED_POS        (7)

#define BULLET_VELOCITY  7
#define SHIP_VELOCITY    5

enum eGameStatus
{
	ASTEROIDS_MENU = 0x001,
	ASTEROIDS_PLAY = 0x002,
	ASTEROIDS_STOP = 0x003
};


struct __stPlayer
{
	int id;
	int x;
	int y;
	int z;
	int w;
	int h;
	int active;
};
typedef struct __stPlayer PLAYER;


PLAYER 	Player0,
		FBullet,
		PowerShip,
		Fire,
		Asteroid1,
		Asteroid2,
		Asteroid3,
		Asteroid4,
		Asteroid5;

int id_spr_splash;
int id_spr_gameover;
int id_spr_bonus;
int id_spr_winner;


int game_status;
char str[80];

int Dificulty;
int Enemy_Velocity;
int Energy;
int Level;
int Score;
int Level_Score;
int Bonus;
int NextLevel;

int timer[10];
int Rand_X;
int Rand_Y;
int	Rand_Value;
int is_cd_playing;

static jo_sound snd_bullet;
static jo_sound snd_xplot;

int idx_std=MAXMAP-2;
int idx_in=0;

int Run;
int Debug_Show;

int Render_Bullet;
int Render_Bullet_Timer;

int Render_Xplot0;
int Render_Xplot1;

struct __stSaturnSystem
{
	int RAM;
	int VRAM;
	int Language;
}Environment;


#endif



