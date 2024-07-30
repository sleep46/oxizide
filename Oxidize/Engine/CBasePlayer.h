#pragma once
#ifndef __CBASEPLAYER_H__
#define __CBASEPLAYER_H__

#ifndef __GAME_H__
#include "Game\Game.h"
#endif

#ifndef __CCOMPONENT_H__
#include "CComponent.h"
#endif

#ifndef __CBASEENTITY_H__
#include "CBaseEntity.h"
#endif

class CBaseEntity;

class CPlayerModel : private CSingleton<CPlayerModel>
{
public:

	Vector3 GetPosition( void )
	{
		return GetValue<Vector3>( 0x1B8 );
	}
	bool	IsNPC( void )
	{
		return GetValue<bool>( 0x2C8 );
	}
};

class CBaseCombatEntity : public CBaseEntity
{
public:
	Faction		GetFaction( void )		{ return GetValue<Faction>( 0x26C ); }
	LifeState	GetLifeState( void )	{ return GetValue<LifeState>( 0x25C ); }

	float		GetHealth( void )		{ return GetValue<float>( 0x264 ); }
	float		GetMaxHealth( void )	{ return GetValue<float>( 0x268 ); }

};

class CBasePlayer : public CBaseCombatEntity
{
public: // pointers

	CPlayerModel* GetModel( void ) { return GetValue<CPlayerModel*>( 0x680 ); }

public: // simple info
	System::String* displayName( void )		{ return GetValue<System::String*>( 0x938 ); }
	bool			wasSleeping( void )		{ return GetValue<bool>( 0x6F1 ); }
	bool			wasDead( void )			{ return GetValue<bool>( 0x6F3 ); }
};

class CLocalPlayer : private CSingleton<CLocalPlayer>
{
public:

	float			GetLastDeathTime( void );
	CBasePlayer*	GetBasePlayer( void );
};

inline CLocalPlayer* g_pLocalPlayer = 0;

#endif