#pragma once
class cMenu {
public:
	void InitStyle( );
	void RenderMenu( );
public:
	bool ShowMenu = true;
};
inline cMenu* Menu = new cMenu( );