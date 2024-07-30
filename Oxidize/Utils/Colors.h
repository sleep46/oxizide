#pragma once

struct Color 
{
	Color( BYTE rr, BYTE gg, BYTE bb )
	{
		r = rr;
		g = gg;
		b = bb;
	}

	Color( void ) {};

	BYTE r;
	BYTE g;
	BYTE b;
};

inline Color GenerateLightColor(void ) 
{
	// Seed the random number generator with current time
	std::srand( static_cast<unsigned int>(std::time( nullptr )) );

	// Generate random RGB values for a light color
	BYTE r = static_cast<BYTE>(std::rand() % 106 + 150);  // Red component (150 to 255)
	BYTE g = static_cast<BYTE>(std::rand() % 106 + 150);  // Green component (150 to 255)
	BYTE b = static_cast<BYTE>(std::rand() % 106 + 150);  // Blue component (150 to 255)

	return Color( r, g, b );
}

struct CGenRandomColor
{
	CGenRandomColor(void)
	{
		color = GenerateLightColor();
	}

	Color color;
};