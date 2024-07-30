#pragma once
#include "../includes.h"
#include "../Engine/Game/Game.h"


enum TextAlign
{
	TextAlign_Left,
	TextAlign_Right,
	TextAlign_Center,
};

class CRender
{
public:
	CRender( void );
	~CRender( void );

public:
	void Init_WorkPos( ImVec2 vWorkPos);
	void Init_DrawList( ImDrawList* pDrawList );

	void AddRect( Vector2 vMin, Vector2 vMax, ImU32 color, float thickness = 1.0f);
	void AddText( Vector2 vPos, TextAlign Align, ImU32 color, LPCSTR lpFormat, ... );

public:

	ImVec2		m_vWorkPos;
	ImDrawList* m_pDrawList;
};

inline CRender* g_pRender = new CRender();

