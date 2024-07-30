#pragma once
#ifndef __CSINGLETON_H__
#define __CSINGLETON_H__

#include <windows.h>
#include <atlstr.h>
#include <cstdint>

#include "..\Driver\kinterface.h"

template <class T>
class CSingleton
{
public:

	bool IsValid( void )
	{
		return this != NULL;
	}

	template <typename C>
	C GetValue( DWORD dwOffs )
	{
		return ReadPhysMemory<C>( (uintptr_t)((uintptr_t)this + dwOffs) );
	}

	bool GetValue( PVOID Buffer, DWORD dwOffs, SIZE_T Size )
	{
		return ReadPhysMemory( (uintptr_t)((uintptr_t)this + dwOffs), (PVOID)Buffer, Size );
	}

	bool GetValue( PVOID Buffer, PVOID dwAddr, SIZE_T Size )
	{
		return ReadPhysMemory( (uintptr_t)dwAddr, (PVOID)Buffer, Size );
	}
};

#endif
