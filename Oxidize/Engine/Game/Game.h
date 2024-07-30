#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include <windef.h>
#include "Offsets.h"
#include "System.h"
#include "Enumerations.h"

#ifndef __VECTOR_H__
#include "vector.h"
#endif

namespace IGame
{
	inline Vector2		vRustWindow = {};
	inline Matrix4x4*	ViewMatrix = NULL;
}

#endif