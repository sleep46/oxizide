#include "..\includes.h"
#include "CBasePlayer.h"

float CLocalPlayer::GetLastDeathTime( void )
{
	if (!IsValid())
		return 0.0f;

	return GetValue<float>( 0x18 );
}

CBasePlayer* CLocalPlayer::GetBasePlayer( void )
{
	if (!IsValid())
		return nullptr;

	return GetValue<CBasePlayer*>( 0xB8 )->GetValue< CBasePlayer*>( NULL );
}
