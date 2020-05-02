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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sgl.h>
#include <jo/jo.h>
#include "global.h"
#include "asteroid.h"

/////////////////////////////////////////////////////////////////////////////
// Function: Exec_Assembler_Hitachi_SH2_CPU                                //
// Summary : HITACHI SH-2 MASTER CPU TEST                                  //
/////////////////////////////////////////////////////////////////////////////

void Exec_Assembler_Hitachi_SH2_CPU(void)
{

	asm ("		.ORG    H'1006 			");
	asm ("		MOV   	#H'7B1 , R1    	");  // STAGES=3 CYCLES=1
	asm ("		MOV   	#H'7C4 , R5    	");  // STAGES=3 CYCLES=1
	asm ("		NOT   	   R5  , R1  	");
	asm ("		ADD     #–1    , R0   	");  // STAGES=3 CYCLES=1
	asm ("		TST     R0     , R0  	");  // STAGES=6 CYCLES=1
	asm ("		SETT                	");
	asm ("		NEG     R1     , R0    	");
    asm ("		.ALIGN  2     			");
	asm ("		CLRMAC         			");
	asm ("		CLRT         			");
	asm ("DATAS1:	               		");
	asm ("		MOV		#H'4D3 , R0     "); // STAGES=3 CYCLES=1
	asm ("		MOV   	#H'7C3 , R1    	"); // STAGES=3 CYCLES=1
	asm ("OPERATION:   					");
	asm ("		ADD   	    R0 , R1  	"); // STAGES=3 CYCLES=1
	asm ("		NOT   	    R0 , R1  	");
    asm ("		STS 	  MACH , R1		"); // STAGES=4 CYCLES=1
    asm ("		SUB         R0 , R1    	");
	asm ("		MULU   	    R2 , R0    	");	// STAGES=6/7*3 CYCLES=1
	asm ("NOP          					");
	asm ("TARGET:      					");
	asm ("		.ALIGN  4     			");
	asm ("		SETT                	");
	asm ("		OR         R5 , R0    	");
	asm ("DATAS2:	               		");
	asm ("		MOV		#H'00A , R0     "); // STAGES=3 CYCLES=1
	asm ("		MOV   	#H'01C , R1    	");	// STAGES=3 CYCLES=1
	asm ("		ADD   	    R5 , R1    	");	// STAGES=3 CYCLES=1
	asm ("		LDC         R1 , SR  	");
	asm ("		STS       MACL , R0		"); // STAGES=4 CYCLES=1
	asm ("		SETT                	");
    asm ("		MOV		#H'00A , R7     ");	// STAGES=3 CYCLES=1
	asm ("		MOV   	#H'01C , R8    	");
	asm ("		MULU   	    R7 , R8    	");	// STAGES=6/7*3 CYCLES=1
	asm ("		STS 	  MACL , R7		");
	asm ("		STS 	  MACH , R5		"); // STAGES=4 CYCLES=1
	asm ("		NEG         R5 , R0    	");
	asm ("		SUB         R5 , R1    	");
	asm ("NOP          					");

}


/////////////////////////////////////////////////////////////////////////////
// Function: Exec_Game                                                     //
// Summary : Entry point on game                                           //
/////////////////////////////////////////////////////////////////////////////

void Exec_Game(void)
{

#ifdef MAZINGER_DEBUG

	//MASTER SH-2 TEST:
	Exec_Assembler_Hitachi_SH2_CPU();

#endif

	//Initialize Jo-Engine
	jo_core_init(JO_COLOR_Black);

	SetBackground();
	Awake();

	game_status = ASTEROIDS_MENU;

	Environment.RAM = jo_memory_usage_percent();
	Environment.VRAM = jo_sprite_usage_percent();
	Environment.Language = slGetLanguage();

	jo_core_add_slave_callback(Splash);	 // SLAVE  CPU HITACHI SH2
	jo_core_add_slave_callback(Frame);	 // SLAVE  CPU HITACHI SH2
	jo_core_add_callback(Update);        // MASTER CPU HITACHI SH2

	jo_core_run();
}


/////////////////////////////////////////////////////////////////////////////
// Function: load_background                                               //
// Summary : Set background TGA                                            //
/////////////////////////////////////////////////////////////////////////////

void SetBackground(void)
{
    jo_img bg;

    bg.data = JO_NULL;
    jo_tga_loader(&bg, "ASSETS", "LEVEL01.TGA", JO_COLOR_Black);
    jo_set_background_sprite(&bg, 0, 0);
    jo_free_img(&bg);
}

void Playfield()
{
   int c;
   int star_x;
   int star_y;

   for(c=0; c<=20; c++)
   {
      star_x = rand()%510; star_y = rand()%200;
      jo_put_pixel_in_background(star_x,star_y,JO_COLOR_Yellow);
   }

}



/////////////////////////////////////////////////////////////////////////////
// Function: Collision                                                     //
// Summary : Collider event                                                //
/////////////////////////////////////////////////////////////////////////////

int Collision(PLAYER *A,PLAYER *B)
{
	if(!A->active) return 0;
	if(!B->active) return 0;
	return (int) jo_hitbox_detection(A->id,A->x,A->y,B->id,B->x,B->y);
}


/////////////////////////////////////////////////////////////////////////////
// Function: Debug                                                         //
// Summary : Debugger game                                                 //
/////////////////////////////////////////////////////////////////////////////
void Debug(void)
{

	if (jo_is_pad1_key_pressed(JO_KEY_B))
	{
		 unsigned short  *ptr;
		 int l=7;

		 Debug_Show = TRUE;

         jo_printf(7,0, "VDP2 VRAM A0      : 0x%X  ", VDP2_VRAM_A0);
		 jo_printf(7,1, "RAM memory usage  : %d%%  ", Environment.RAM);
		 jo_printf(7,2, "Video memory usage: %d%%  ", Environment.VRAM);
		 jo_printf(7,3, "Peripheral 1 mode : 0x%X  ", slGetPortMode1());
		 jo_printf(7,4, "User Language     : %d    ", slGetLanguage());
		 jo_printf(7,5, "Sound mode        : %d    ", slGetSoundOutput());
		 jo_printf(7,6, "SMPC system status: 0x%X  ", Smpc_Status); //Microcontroller Hitachi HD404920

		 for (ptr = (unsigned short *)JO_VDP1_REG; ptr <= (unsigned short *)JO_VDP1_LAST_REG; ++ptr)
		     {
				jo_printf(7,l, "VDP1: 0x%X = 0x%X", (int)ptr, *ptr);
				l++;
			 }

		for (ptr = (unsigned short *)JO_VDP2_REG; ptr <= (unsigned short *)JO_VDP2_LAST_REG; ++ptr)
		     {
				jo_printf(7,l, "VDP2: 0x%X = 0x%X", (int)ptr, *ptr);
				l++;
			 }


	}
	else Debug_Show = FALSE;

}



/////////////////////////////////////////////////////////////////////////////
// Function: Frame                                                         //
// Summary : BlitSurface all graphics on game                              //
/////////////////////////////////////////////////////////////////////////////

void Frame(void)
{
	if(game_status != ASTEROIDS_PLAY || !Run) return;

	//Clear Playfield area
    jo_clear_screen();

	//Asteroids
	jo_sprite_draw3D2(Asteroid1.id,Asteroid1.x,Asteroid1.y,Asteroid1.z);
	jo_sprite_draw3D2(Asteroid2.id,Asteroid2.x,Asteroid2.y,Asteroid2.z);
	jo_sprite_draw3D2(Asteroid3.id,Asteroid3.x,Asteroid3.y,Asteroid3.z);
	jo_sprite_draw3D2(Asteroid4.id,Asteroid4.x,Asteroid4.y,Asteroid4.z);
	jo_sprite_draw3D2(Asteroid5.id,Asteroid5.x,Asteroid5.y,Asteroid5.z);

	//Player
	jo_sprite_draw3D2(Player0.id,Player0.x,Player0.y,Player0.z);

	//Animations
	PowerAnimation();
	BulletAnimation();
	FireAnimation();

DEBUG_GO

	//Write messages
	//sprintf(str,"  x: %d   y: %d   status: %d",Player0.x,Player0.y,game_status);
	sprintf(str,"Score: %d   Level: %d   Energy: %d",Score,Level,Energy);
	jo_printf(0,0,str);

DEBUG_END


#ifdef MAZINGER_DEBUG
	Debug();
#endif

}

/////////////////////////////////////////////////////////////////////////////
// Function: play_song                                                     //
// Summary : Music of game (SATURN CD)                                     //
/////////////////////////////////////////////////////////////////////////////

void play_song(void)
{

	if(is_cd_playing) return;

	switch(Level)
	{
		case 1:
		{
   		jo_audio_play_cd_track(TRACK_LEVEL1,TRACK_LEVEL1, CD_LOOP);
   		is_cd_playing = TRUE;
		break;
		}

	}


}

/////////////////////////////////////////////////////////////////////////////
// Function: OnGameOver                                                    //
// Summary : Game Over event controller                                    //
/////////////////////////////////////////////////////////////////////////////

void OnGameOver()
{
   	jo_audio_stop_cd();
   	is_cd_playing = FALSE;
   	Run = FALSE;

    jo_sprite_draw3D(id_spr_gameover,0,0,500);

	timer[1]++;
	if(timer[1] > 500)
	{
        Score+=Level_Score;
		game_status = ASTEROIDS_MENU;
		timer[1]=0;
	}

}

/////////////////////////////////////////////////////////////////////////////
// Function: Update                                                        //
// Summary : Update on game loop                                           //
/////////////////////////////////////////////////////////////////////////////

void Update()
{
	if(game_status != ASTEROIDS_PLAY) return;

RUN

	play_song();

DEBUG_GO

	//-------------------------Asteroids IA------------------------------------------------------//
	timer[0]++;
	if(timer[0] > 5)
	{
		if (Asteroid1.x < Player0.x) Asteroid1.x += Enemy_Velocity;
		if (Asteroid1.x > Player0.x) Asteroid1.x -= Enemy_Velocity;
		if (Asteroid1.y < Player0.y) Asteroid1.y += Enemy_Velocity;
		if (Asteroid1.y > Player0.y) Asteroid1.y -= Enemy_Velocity;

	timer[0]=0;
	}

	//--------------------------Asteroid 2-----------------------//
	Rand_X = rand()%5;
	Rand_Value = rand()%7;
	if (Rand_X == 0) if (Asteroid2.x < SCREEN_H_MAX) Asteroid2.x += Rand_Value;
	if (Rand_X == 1) if (Asteroid2.x > SCREEN_H_MIN)   Asteroid2.x -= Rand_Value;
	Rand_Y = rand()%5;
	Rand_Value = rand()%7;
	if (Rand_Y == 0) if (Asteroid2.y < SCREEN_V_MAX) Asteroid2.y += Rand_Value;
	if (Rand_Y == 1) if (Asteroid2.y > SCREEN_V_MIN)   Asteroid2.y -= Rand_Value;

	//--------------------------Asteroid 3-----------------------//
	Rand_X = rand()%5;
	Rand_Value = rand()%7;
	if (Rand_X == 0) if (Asteroid3.x < SCREEN_H_MAX) Asteroid3.x += Rand_Value;
	if (Rand_X == 1) if (Asteroid3.x > SCREEN_H_MIN)   Asteroid3.x -= Rand_Value;
	Rand_Y = rand()%5;
	Rand_Value = rand()%7;
	if (Rand_Y == 0) if (Asteroid3.y < SCREEN_V_MAX) Asteroid3.y += Rand_Value;
	if (Rand_Y == 1) if (Asteroid3.y > SCREEN_V_MIN)   Asteroid3.y -= Rand_Value;

	//--------------------------Asteroid 4-----------------------//
	Rand_X = rand()%5;
	Rand_Value = rand()%7;
	if (Rand_X == 0) if (Asteroid4.x < SCREEN_H_MAX) Asteroid4.x += Rand_Value;
	if (Rand_X == 1) if (Asteroid4.x > SCREEN_H_MIN)   Asteroid4.x -= Rand_Value;
	Rand_Y = rand()%5;
	Rand_Value = rand()%7;
	if (Rand_Y == 0) if (Asteroid4.y < SCREEN_V_MAX) Asteroid4.y += Rand_Value;
	if (Rand_Y == 1) if (Asteroid4.y > SCREEN_V_MIN)   Asteroid4.y -= Rand_Value;

	//--------------------------Asteroid 5-----------------------//
	Rand_X = rand()%5;
	Rand_Value = rand()%7;
	if (Rand_X == 0) if (Asteroid5.x < SCREEN_H_MAX) Asteroid5.x += Rand_Value;
	if (Rand_X == 1) if (Asteroid5.x > SCREEN_H_MIN)   Asteroid5.x -= Rand_Value;
	Rand_Y = rand()%5;
	Rand_Value = rand()%7;
	if (Rand_Y == 0) if (Asteroid5.y < SCREEN_V_MAX) Asteroid5.y += Rand_Value;
	if (Rand_Y == 1) if (Asteroid5.y > SCREEN_V_MIN)   Asteroid5.y -= Rand_Value;


DEBUG_END


	//Joystick controller------------------------------------------------------------------------//

	if (jo_is_pad1_key_pressed(JO_KEY_UP))
	{
		if(Player0.y > SCREEN_V_MIN) Player0.y-= SHIP_VELOCITY;
	}

	if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
	{
		if(Player0.y < SCREEN_V_MAX) Player0.y+= SHIP_VELOCITY;
	}

	if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
	{
		if(Player0.x < SCREEN_H_MAX) Player0.x+= SHIP_VELOCITY;
	}

	if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
	{
		if(Player0.x > SCREEN_H_MIN) Player0.x-= SHIP_VELOCITY;
	}

	//----------------------Saturn PAD button events-------------------//

	if (jo_is_pad1_key_pressed(JO_KEY_A))
	{
		jo_audio_play_sound_on_channel(&snd_bullet, 0);
		FBullet.x = Player0.x + 14;
		FBullet.y = Player0.y - 7;
		Render_Bullet = TRUE;
	} else Render_Bullet = FALSE;

	if (jo_is_pad1_key_pressed(JO_KEY_C))
	{
		jo_audio_stop_cd();
	 	jo_goto_boot_menu();
	}

	if (jo_is_pad1_key_pressed(JO_KEY_X))
	{
		jo_core_suspend(); //PAUSE, press [START] to continue...
	}


	//----------------------------COLLISIONS-LOGIC-----------------------------------------------//
    Render_Xplot0 = FALSE; Render_Xplot1 = FALSE;

	if(Collision(&Player0, TYPE &Asteroid1)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Energy-=Dificulty; Render_Xplot0 = TRUE; }
	if(Collision(&Player0, TYPE &Asteroid2)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Energy-=Dificulty; Render_Xplot0 = TRUE; }
	if(Collision(&Player0, TYPE &Asteroid3)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Energy-=Dificulty; Render_Xplot0 = TRUE; }
	if(Collision(&Player0, TYPE &Asteroid4)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Energy-=Dificulty; Render_Xplot0 = TRUE; }
	if(Collision(&Player0, TYPE &Asteroid5)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Energy-=Dificulty; Render_Xplot0 = TRUE; }

	if(Collision(&FBullet, TYPE &Asteroid1)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Level_Score+=Bonus; Score+=Bonus; Render_Xplot1 = TRUE; }
	if(Collision(&FBullet, TYPE &Asteroid2)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Level_Score+=Bonus; Score+=Bonus; Render_Xplot1 = TRUE; }
	if(Collision(&FBullet, TYPE &Asteroid3)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Level_Score+=Bonus; Score+=Bonus; Render_Xplot1 = TRUE; }
	if(Collision(&FBullet, TYPE &Asteroid4)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Level_Score+=Bonus; Score+=Bonus; Render_Xplot1 = TRUE; }
	if(Collision(&FBullet, TYPE &Asteroid5)) { jo_audio_play_sound_on_channel(&snd_xplot,1); Level_Score+=Bonus; Score+=Bonus; Render_Xplot1 = TRUE; }


END_RUN


    //Game rules----------------------------------------------------------//

	if(Level_Score >= NextLevel)
	{
		OnWinner();
	}

    if(Energy <= 0)
    {
    	OnGameOver();
    }





}

/////////////////////////////////////////////////////////////////////////////
// Function: Awake                                                         //
// Summary : Load all assets                                               //
/////////////////////////////////////////////////////////////////////////////

void Awake()
{

	id_spr_splash = jo_sprite_add_tga("ASSETS", "TERRA.TGA", JO_COLOR_Transparent);
	id_spr_gameover = jo_sprite_add_tga("ASSETS", "GAOVER.TGA", JO_COLOR_Transparent);
	id_spr_bonus = jo_sprite_add_tga("ASSETS", "BONUS.TGA", JO_COLOR_Transparent);
	id_spr_winner = jo_sprite_add_tga("ASSETS", "WINNER.TGA", JO_COLOR_Transparent);

	Player0.id = jo_sprite_add_tga("ASSETS", "SHIP.TGA", JO_COLOR_Transparent);
	FBullet.id = jo_sprite_add_tga("ASSETS", "BULLET.TGA", JO_COLOR_Transparent);
	PowerShip.id = jo_sprite_add_tga("ASSETS", "POWER.TGA", JO_COLOR_Transparent);
	Fire.id = jo_sprite_add_tga("ASSETS", "FIRE.TGA", JO_COLOR_Transparent);

	Asteroid1.id = jo_sprite_add_tga("ASSETS", "ENEMY1.TGA", JO_COLOR_Transparent);
	Asteroid2.id = jo_sprite_add_tga("ASSETS", "ENEMY2.TGA", JO_COLOR_Transparent);
	Asteroid3.id = jo_sprite_add_tga("ASSETS", "ENEMY3.TGA", JO_COLOR_Transparent);
	Asteroid4.id = jo_sprite_add_tga("ASSETS", "ENEMY1.TGA", JO_COLOR_Transparent);
	Asteroid5.id = jo_sprite_add_tga("ASSETS", "ENEMY2.TGA", JO_COLOR_Transparent);

	jo_audio_load_pcm("BULLET.PCM", JoSoundMono16Bit, &snd_bullet);
	jo_audio_load_pcm("XPLOT.PCM", JoSoundMono16Bit, &snd_xplot);


}

/////////////////////////////////////////////////////////////////////////////
// Function: Start                                                         //
// Summary : Initialize game                                               //
/////////////////////////////////////////////////////////////////////////////

void Start()
{

	Player0.x   = 141;
	Player0.y   = 107;
	Player0.z   = 500;
	Player0.w   = 32;
	Player0.h   = 32;
	Player0.active = TRUE;

	FBullet.x   = 141;
	FBullet.y   = 107;
	FBullet.z   = 500;
	FBullet.w   = 8;
	FBullet.h   = 8;
	FBullet.active = TRUE;

	PowerShip.x   = 141;
	PowerShip.y   = 107;
	PowerShip.z   = 500;
	PowerShip.w   = 16;
	PowerShip.h   = 16;
	PowerShip.active = TRUE;

	Fire.x   = 141;
	Fire.y   = 107;
	Fire.z   = 500;
	Fire.w   = 32;
	Fire.h   = 32;
	Fire.active = TRUE;

	Asteroid1.x   = 9;
	Asteroid1.y   = 70;
	Asteroid1.z   = 500;
	Asteroid1.w   = 32;
	Asteroid1.h   = 32;
	Asteroid1.active = TRUE;

	Asteroid2.x   = 57;
	Asteroid2.y   = 70;
	Asteroid2.z   = 500;
	Asteroid2.w   = 32;
	Asteroid2.h   = 32;
	Asteroid2.active = TRUE;

	Asteroid3.x   = 165;
	Asteroid3.y   = 70;
	Asteroid3.z   = 500;
	Asteroid3.w   = 32;
	Asteroid3.h   = 32;
	Asteroid3.active = TRUE;

	Asteroid4.x   = 207;
	Asteroid4.y   = 30;
	Asteroid4.z   = 500;
	Asteroid4.w   = 32;
	Asteroid4.h   = 32;
	Asteroid4.active = TRUE;

	Asteroid5.x   = 267;
	Asteroid5.y   = 50;
	Asteroid5.z   = 500;
	Asteroid5.w   = 32;
	Asteroid5.h   = 32;
	Asteroid5.active = TRUE;


	switch(Level)
	{

		case 1:
		{
			Dificulty      = 1;
			Enemy_Velocity = 3;
			Bonus          = 3;
			Energy         = 300;
			Level_Score    = 0;
			NextLevel      = 10;
			break;
		}

		case 2:
		{
			Dificulty      = 1;
			Enemy_Velocity = 3;
			Bonus          = 3;
			Energy         = 300;
			Level_Score    = 0;
			NextLevel      = 10;
			break;
		}

		case 3:
		{
			Dificulty      = 1;
			Enemy_Velocity = 3;
			Bonus          = 3;
			Energy         = 300;
			Level_Score    = 0;
			NextLevel      = 10;
			break;
		}

		case 4:
		{
			Dificulty      = 1;
			Enemy_Velocity = 3;
			Bonus          = 3;
			Energy         = 300;
			Level_Score    = 0;
			NextLevel      = 10;
			break;
		}

		case 5:
		{
			Dificulty      = 1;
			Enemy_Velocity = 3;
			Bonus          = 3;
			Energy         = 300;
			Level_Score    = 0;
			NextLevel      = 10;
			break;
		}

	}


	srand(time(NULL));

	is_cd_playing = FALSE;

	Render_Bullet = 0;
	Render_Bullet_Timer = 0;

	Render_Xplot0 = 0;
	Render_Xplot1 = 0;

	Debug_Show = FALSE;


}


/////////////////////////////////////////////////////////////////////////////
// Function: FireAnimation                                                 //
// Summary : Fire animation for collisions                                 //
/////////////////////////////////////////////////////////////////////////////

void FireAnimation()
{
	if(Render_Xplot0)
	{
		int r,t;

		Fire.x = Player0.x;
		Fire.y = Player0.y;

		for (r = 0; r <= 5; r++) //velocity
		{
			Fire.x+= rand() % 7;
			Fire.y+= rand() % 7;
			jo_sprite_draw3D2(Fire.id,Fire.x,Fire.y,Fire.z);
		}
	}

	if(Render_Xplot1)
	{
		int r,t;

		Fire.x = FBullet.x;
		Fire.y = FBullet.y;

		for (r = 0; r <= 5; r++) //velocity
		{
			Fire.x+= rand() % 7;
			Fire.y+= rand() % 7;
			jo_sprite_draw3D2(Fire.id,Fire.x,Fire.y,Fire.z);
		}
	}




}

/////////////////////////////////////////////////////////////////////////////
// Function: BulletAnimation                                               //
// Summary : Bullet of ship                                                //
/////////////////////////////////////////////////////////////////////////////

void BulletAnimation()
{
	if (Render_Bullet && (Render_Bullet_Timer == 0))
	    Render_Bullet_Timer = 20;

	if(Render_Bullet_Timer > 0)
	{
	    FBullet.active = TRUE;
		FBullet.y -= BULLET_VELOCITY;
		jo_sprite_draw3D2(FBullet.id,FBullet.x,FBullet.y,FBullet.z);
		Render_Bullet_Timer--;
	}
	else
		FBullet.active = FALSE;

}



/////////////////////////////////////////////////////////////////////////////
// Function: PowerAnimation                                                //
// Summary : Energy power of ship                                          //
/////////////////////////////////////////////////////////////////////////////

void PowerAnimation()
{
		int r,t;

		PowerShip.x = Player0.x;
		PowerShip.y = Player0.y + 20;

		for (r = 0; r <= 3; r++) //velocity
		{
			PowerShip.x+= rand() % 7;
			PowerShip.y+= rand() % 7;
			jo_sprite_draw3D2(PowerShip.id,PowerShip.x,PowerShip.y,PowerShip.z);
		}

}


void OnWinner()
{

   	jo_audio_stop_cd();
   	is_cd_playing = FALSE;
   	Run = FALSE;

    jo_sprite_draw3D(id_spr_bonus,0,0,500);

	timer[1]++;
	if(timer[1] > 500)
	{
		if(Level >= 5) OnEndGame();

        Score+=Level_Score;
		if(Level < 5) Level++;
		Start();
		Run = TRUE;
		timer[1]=0;
	}

}


void OnEndGame()
{

    jo_sprite_draw3D(id_spr_winner,0,0,500);

	timer[1]++;
	if(timer[1] > 500)
	{
        Score+=Level_Score;
		timer[1]=0;
		jo_goto_boot_menu();
	}



}


/////////////////////////////////////////////////////////////////////////////
// Function: Splash                                                        //
// Summary : Main screen game                                              //
/////////////////////////////////////////////////////////////////////////////

void Splash()
{
	if(game_status != ASTEROIDS_MENU) return;

 	if(!is_cd_playing)
    {
        jo_audio_play_cd_track(TRACK_INTRO,TRACK_INTRO, CD_LOOP);
        is_cd_playing = TRUE;
    }

	Level = 1;
	Score = 0;

    jo_clear_screen();
	jo_printf(1,12,"(c)Nova32, Rolando Fernandez Benavidez 2019.");
	jo_sprite_draw3D2(id_spr_splash,0,0,500);

	if (jo_is_pad1_key_pressed(JO_KEY_START))
	{
		jo_clear_screen();
		game_status = ASTEROIDS_PLAY;
		Start();
		jo_audio_stop_cd();
		is_cd_playing = FALSE;
		Run = TRUE;
	}

}

