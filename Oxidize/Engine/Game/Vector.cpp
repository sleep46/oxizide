#include "../includes.h"
#include "Vector.h"
#include "Game.h"

bool Vector3::W2S( Vector2& ScreenPos )
{
	if (!IGame::ViewMatrix || !this)
		return false;

	Vector3 EntityPos = Vector3( x, y, z );
	Vector3 TransVec = Vector3( IGame::ViewMatrix->_14, IGame::ViewMatrix->_24, IGame::ViewMatrix->_34 );
	Vector3 RightVec = Vector3( IGame::ViewMatrix->_11, IGame::ViewMatrix->_21, IGame::ViewMatrix->_31 );
	Vector3 UpVec = Vector3( IGame::ViewMatrix->_12, IGame::ViewMatrix->_22, IGame::ViewMatrix->_32 );
	float w = Math::Dot( TransVec, EntityPos ) + IGame::ViewMatrix->_44;
	if (w < 0.098f) return false;
	float y = Math::Dot( UpVec, EntityPos ) + IGame::ViewMatrix->_42;
	float x = Math::Dot( RightVec, EntityPos ) + IGame::ViewMatrix->_41;
	ScreenPos = Vector2( (IGame::vRustWindow.x / 2) * (1.f + x / w), (IGame::vRustWindow.y / 2) * (1.f - y / w) );

	if (ScreenPos.x > overlay->Width || ScreenPos.y > overlay->Height || ScreenPos.x < 0 || ScreenPos.y < 0)
		return false;

	return true;
}