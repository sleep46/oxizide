#pragma once
#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "CSingleton.h"

namespace System
{
	class BufferList : CSingleton<BufferList> {
	public:
		int GetSize() {
			return GetValue<int>( 0x10 );
		}

		BufferList* GetBuffer() {
			return GetValue<BufferList*>( 0x18 );
		}

		void* GetArray( int index ) {
			return GetBuffer()->GetValue<void*>( 0x20 + (index * sizeof( void* )) );
		}
	};

	class ListDictionary : CSingleton<ListDictionary> {
	public:
		BufferList* GetValues() {
			return GetValue<BufferList*>( 0x28 );
		}
	};

	class String : CSingleton<String>
	{
	public:

		int		GetLength( void ) { return GetValue<int>( 0x0010 ); }
		LPCSTR	GetString( void )
		{
			WCHAR wStr[ 64 ] {};
			ZeroMemory( wStr, sizeof( wStr ) );
			GetValue( wStr, 0x14, sizeof( wStr ) );
			return CString( wStr );
		}
	};
}

#endif
