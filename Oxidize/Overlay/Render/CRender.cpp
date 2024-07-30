#include "CRender.h"

CRender::CRender( void )
{
	m_vWorkPos = ImVec2( 0, 0 );
	m_pDrawList = NULL;
}

CRender::~CRender( void )
{
}

void CRender::Init_WorkPos( ImVec2 vWorkPos )
{
}

void CRender::Init_DrawList( ImDrawList* pDrawList )
{
	m_pDrawList = pDrawList;
}

void CRender::AddRect( Vector2 vMin, Vector2 vMax, ImU32 color, float thickness )
{
	if (m_pDrawList)
		m_pDrawList->AddRect( ImVec2( vMin.x, vMin.y ), ImVec2( vMin.x + vMax.x, vMin.y + vMax.y ), color, 0, 0, thickness );
}

void CRender::AddText( Vector2 vPos, TextAlign Align, ImU32 color, LPCSTR lpFormat, ... )
{
	CString Buffer;
	va_list list;
	va_start( list, lpFormat );
	Buffer.FormatV( lpFormat, list );
	va_end( list );

	auto text_size = ImGui::CalcTextSize( Buffer );
	auto text_location = vPos;

	if(Align & TextAlign_Center)
		text_location.x -= text_size.x / 2.f;

	if (m_pDrawList)
		m_pDrawList->AddText( ImVec2( text_location.x, text_location.y ), color, Buffer );
}
