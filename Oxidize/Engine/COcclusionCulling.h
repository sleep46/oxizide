#pragma once
#ifndef __COCCLUSIONCULLING_H__
#define __COCCLUSIONCULLING_H__
#include "CSingleton.h"

#ifndef __CCOMPONENT_H__
#include "CComponent.h"
#endif

class COcclusionCulling : public CComponent
{
public:



};

inline COcclusionCulling* g_pOcclusionCulling = NULL;

#endif