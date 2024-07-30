#pragma once
#ifndef __CCOMPONENT_H__
#define __CCOMPONENT_H__

#include "..\Engine\Game\CSingleton.h"

class CComponent : public CSingleton<CComponent> {
public:

	LPCSTR GetClassNameA()
	{
		auto classPtr = GetValue<CComponent*>( 0 )->GetValue<CComponent*>( 0x10 );
		if (!classPtr) return "";

		char name[ 128 ];
		ZeroMemory( name, 128 );
		GetValue( name, classPtr, 128 );

		return name;
	}

	bool IsFrom( const char* fromClassName ) {
		if (!this)
			return false;

		const char* className = this->GetClassNameA();
		if (strcmp( className, fromClassName ) != NULL)
			return false;

		return true;
	}
};

#endif
