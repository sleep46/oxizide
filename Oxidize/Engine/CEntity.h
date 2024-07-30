#pragma once
#ifndef __CENTITY_H__
#define __CENTITY_H__

#include "../includes.h"

#include "CBaseNPC.h"
#include "Game\Game.h"

class CBasePlayer;
class CPlayerModel;
class CModel;

struct _stBaseEntity
{
	char		szClassName[ 128 ] {};
	Vector3		position {};
	float		distance {};
	CModel*		pModel {};
	Color		cColors;
	DWORD		dwTraitFlags {};
};

struct _stBaseAnimal : _stBaseEntity
{
	AiStatistics	cAiStatistics;
};

struct _stBasePlayer : _stBaseEntity
{
	CBasePlayer* pBasePlayer {};
	CPlayerModel* pPlayerModel {};

	LifeState	lifeState {};
	Faction		faction {};

	float		health = NULL;
	float		maxHealth = NULL;
	char		szName[ 128 ] {};

	bool		IsHumanNPC = false;
	bool		isSleeping = false;
	bool		isLocal = false;
	bool		isDead = false;
};

struct _stBaseNPC : _stBasePlayer
{

};


class CBaseEntity;
class CModel;

class CEntity 
{
public:
	CEntity( void );
	virtual ~CEntity( void );

public:
	void DrawBones( ImDrawList *drawList, CModel* pModel, ImU32 col );
	void EntityList( void );

public:
	void Update( void );
	void Settings( void );
	void OnGameEnter( void );
	void OnGameLeave( void );
	void RenderDrawing( void );
	void Initialize( void );

public:

	bool							bBones;
	bool							bBox2D;
	float							fRenderDistance;
	std::map<std::string, bool>		mapOtherEntities;
};

inline std::vector<_stBaseEntity>	g_vecBaseEntity {};
inline std::vector<_stBaseAnimal>	g_vecBaseAnimal {};
inline std::vector<_stBasePlayer>	g_vecBasePlayer {};
inline CEntity*						g_pEntity = new CEntity();

#endif
