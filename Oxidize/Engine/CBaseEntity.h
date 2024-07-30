#pragma once
#ifndef __CBASEENTITY_H__
#define __CBASEENTITY_H__

#ifndef __GAME_H__
#include "Game\Game.h"
#endif

#ifndef __CCOMPONENT_H__
#include "CComponent.h"
#endif

#ifndef __CHUMANNPC_H__
#include "CHumanNPC.h"
#endif

#ifndef __CMODEL_H__
#include "CModel.h"
#endif

#ifndef __CBASENPC_H__
#include "CBaseNPC.h"
#endif

class CBasePlayer;
class CBaseEntity : public CComponent
{
public:

	CBaseNPC*		ToBaseNPC( void ) { return (CBaseNPC*)this; }
	CHumanNPC*		ToHumanNPC( void )	{ return (CHumanNPC*)this; }
	CBasePlayer*	ToPlayer( void )	{ return (CBasePlayer*)this; }
	CModel*			GetModel( void )	{ return GetValue<CModel*>( 0xB8 ); }
};

#endif