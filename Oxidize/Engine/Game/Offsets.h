#pragma once
#ifndef __OFFSETS_H__
#define __OFFSETS_H__

#include <stdint.h>

namespace IOffset
{
	// GameAssembly.dll 
	inline uintptr_t dwLocalPlayer = 60534720;			// LocalPlayer_TypeInfo
	inline uintptr_t dwBaseNetworkable = 60670200;		// BaseNetworkable_TypeInfo
	inline uintptr_t dwCameraManager = 60547360;		// MainCamera_TypeInfo
}

#endif