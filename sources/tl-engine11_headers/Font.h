/**************************************************************************************************
	Font.h

	Definition of the Font class, a class defining a single font for text output
**************************************************************************************************/

#ifndef TL_FONT_H_INCLUDED
#define TL_FONT_H_INCLUDED

#include <string>   // Standard libraries
using std::string;

#include "Common.h" // Common TL-Engine definitions

namespace tle {     // TL-Engine interface


// Colours for fonts
const unsigned int White     = 0xffffffff;
const unsigned int Black     = 0xff000000;
const unsigned int Red       = 0xffff0000;
const unsigned int Green     = 0xff00ff00;
const unsigned int Blue      = 0xff0000ff;
const unsigned int Cyan      = 0xff00ffff;
const unsigned int Magenta   = 0xffff00ff;
const unsigned int Yellow    = 0xffffff00;
const unsigned int LightGrey = 0xffc0c0c0;
const unsigned int MidGrey   = 0xff808080;
const unsigned int Grey      = 0xff808080;	// Alias for MidGrey
const unsigned int DarkGrey  = 0xff404040;


// Horizontal alignment settings for text drawing
enum HorizontalAlignment
{
	LeftAlign,
	CentreAlign, // Centred horizontally
	RightAlign,
};

// Vertical alignment settings for text drawing
enum VerticalAlignment
{
	TopAlign,
	MiddleAlign, // Centred vertically
	BottomAlign,
};



// Font class declaration
class Font
{
	/*---------------------------------------------------------------------------------------------
		Construction
	---------------------------------------------------------------------------------------------*/
public:
	// No construction needed

	// Destructor
	virtual ~Font() {}


	/*---------------------------------------------------------------------------------------------
		Measuring
	---------------------------------------------------------------------------------------------*/
public:
	// Return the width in pixels of the given text if drawn in this font
	virtual int MeasureTextWidth(const string& text) = 0;

	// Return the Height in pixels of the given text if drawn in this font
	virtual int MeasureTextHeight(const string& text) = 0;


	/*---------------------------------------------------------------------------------------------
		Drawing
	---------------------------------------------------------------------------------------------*/
public:
	// Draw text in this font at the given position. Optionally provide a colour and horizontal / 
	// vertical alignment settings. This text is not permanent, it is drawn for one frame only.
	// Call this function each frame (i.e. in the game loop) for permanent text.
	virtual void Draw(const string& text, int x, int y, unsigned int colour = Black,
		              HorizontalAlignment xAlign = LeftAlign, VerticalAlignment yAlign = TopAlign) = 0;

}; // class Font


} // namespace tle

#endif // TL_FONT_H_INCLUDED
