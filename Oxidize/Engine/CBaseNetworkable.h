#pragma once
#ifndef __CBASENETWORKABLE_H__
#define __CBASENETWORKABLE_H__
#include "Game\Game.h"


class CBaseNetworkable : private CSingleton<CBaseNetworkable>
{
public:

	class CEntityRealm : public CSingleton<CEntityRealm>
	{
	public:
		System::ListDictionary* GetChildren( void )
		{
			return GetValue<System::ListDictionary*>( 0x0010 );
		}
	};

	CEntityRealm* GetEntityRealm( void )
	{
		auto pRealm = GetValue<CEntityRealm*>( 0x00B8 );
		if (!pRealm) return nullptr;

		return pRealm->GetValue<CEntityRealm*>( 0x0000 );
	}
};

inline CBaseNetworkable* g_pBaseNetworkable = 0;

#endif

