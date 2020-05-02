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

#ifndef __ASTEROID_H__
# define __ASTEROID_H__


#define MAZINGER_DEBUG

//Functions prototype:

void Exec_Assembler_Hitachi_SH2_CPU(void);
void OnWinner();
void SetBackground(void);
int  Collision(PLAYER *A,PLAYER *B);
void Stars(void);
void Frame(void);
void play_song(void);
void OnGameOver();
void Update();
void Awake();
void Start();
void Splash();
void Debug(void);
void PowerAnimation();
void BulletAnimation();
void FireAnimation();
void Init_mode7();
void Render_mode7();

#endif
