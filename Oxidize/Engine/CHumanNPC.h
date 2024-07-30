#pragma once
#ifndef __CHUMANNPC_H__
#define __CHUMANNPC_H__


#ifndef __GAME_H__
#include "Game\Game.h"
#endif

class CHumanNPC : public CSingleton< CHumanNPC>
{
public:


};

class CScientistNPC : public CHumanNPC
{
public:

	System::String* deathStatName( void ) { return GetValue<System::String*>( 0xAD0 ); }

};

#endif