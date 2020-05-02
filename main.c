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
*                       A S T E R O I D S - H Y P E R                       *
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
#include <jo/jo.h>

extern void Exec_Game();

void jo_main(void)
{
	Exec_Game();
}

