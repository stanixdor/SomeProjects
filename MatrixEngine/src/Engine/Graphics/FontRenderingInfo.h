//Mikel Fernandez.

#pragma once

#include <string>

namespace Graphics
{
	class Mesh;
	class Texture;

	// Glyph structure
	struct Glyph
	{
		// Character code the glyph represents
		int mCharacterCode;

		// Glyph bounding box - use these as is when generating the character quad
		// they are scaled with respect to the font size above
		float mX0, mY0, mX1, mY1;

		// Texture coordinates in the image
		float mU0, mV0, mU1, mV1;

		// Advance Width
		float mAdvanceWidth;
	};

	struct FontInfo
	{
		/*	FontInfo contains all the info needed to
		build a text mesh where each quad represent a
		glyph (i.e. a character). The information
		stored in the FontInfo structure are:

		1) Font size. This value is passed by the user when processing the
		font file. All of the spacing information is dependent on this value.
		That means that if you wanted to use the same font family at a different
		size, you will have to create a new FontInfo structure.

		2) Line Gap. This value defines the gap to add between two lines of text
		using this font.

		2) Name of the font image (the image data is assumed to
		have been previously saved to file so as not to
		store it into that structure unecessarily). We choose
		not to store a pointer to the image resource so that we don't create
		a unnecessary dependency between the two systems.

		2) Glyph information. All of the information needed to properly create
		a quad containing a character:
		- Texture coordinates (min, max) of the character location in the
		font image specified in 2) in normalized image coordinates with
		bottom left as origin.
		- Advance width: The distance between the left and right sides of the
		character. This is used to properly space the characters when converting
		a string to a text mesh.
		- Kerning information: Extra distance to add or remove (positive, negative)
		that depends on a pair of character ("Fh" is different then "Fm")
		*/

		// Font Size
		float		mFontSize;

		// Line Gap
		float		mLineGap;

		// Name of the font image - assumes that it has been previously saved.
		std::string	mFontFileName;

		// Name of the font image - assumes that it has been previously saved.
		std::string	mFontBitmapFileName;

		// Pointer to the font texture containing all the characters.
		// Note: this should be a resource pointer. 
		Texture * mFontBitmap;

		// First character that this font contains
		int	mFirstCharacterCode;

		// Last character that this font contains
		int mLastCharacterCode;

		// Num character that this font contains.
		// This is the same as mLastCharacterCode - mFirstCharacterCode. 
		int mCharacterCount;

		// Container for all the glyphs
		FontInfo::Glyph * mGlyphContainer;

		// Kerning Table containing the kerning distance for each pairs of glyphs.
		// The table is represented as a 2D array of floats where each index represents
		// the character code of the pair. The array is setup in a way that the data is duplicated
		// for each pair combinations. i.e. mKerningTable[i][j] = mKerningTable[j][i]
		float ** mKerningTable;
	};


	// ------------------------------------------------------------------------
	// \fn		LoadTTF
	// \brief	Loads a TTF file specified by 'fileName' and generates the 
	//			necessary information for displaying font from a texture into 
	//			an user-created FontInfo class. 
	//
	//	\param	fileName		Name of the ttf file to load
	//	\param	start_char		First character (unicode) to load from ttf file.
	//	\param	end_char		Last character (unicode) to load from ttf file.
	//	\param	bitmap_w		Width of the bitmap in which to pack the character data.
	//	\param	bitmap_h		Height of the bitmap in which to pack the character data.
	//	\param	font_size		Size of the font after extracting it from ttf file.
	//	\param	outFontInfo		FontInfo class to hold all data for retrieving characters from a fon texture
	//	\param	saveToTexture	Specifies whether to save the packed characters to image file (PNG).
	//	\param	textureFileName	Name of the texture (if NULL, the path and name of 'fileName' is used). 
	void LoadTTF(const char * fileName,
		int start_char, int end_char,
		int bitmap_width, int bitmap_height,
		float font_size, FontInfo * outputFontInfo,
		bool saveToTexture = true,
		const char * textureFilename = NULL);
}