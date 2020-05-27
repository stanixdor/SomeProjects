#include "FontRenderingInfo.h"
#include "Platform\File\FilePath.h"

#include "Platform\Resource Manager\ResourceManager.h"

// include STB
#pragma warning (disable:4996) //  fopen': This function or variable may be unsafe. 
#define PACK_OVERSAMPLE 1
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stb_image\stb_image_write.h"
#pragma warning (default:4996) //  fopen': This function or variable may be unsafe.

namespace Graphics
{
	// ------------------------------------------------------------------------
	// \fn		PackFontRange
	// \brief	Given an stbtt_pack_range and a stbtt_fontinfo, this function 
	//			will pack each character in the range into a rectangular bitmap
	//			specicified by bitmap_width and bitmap_height. Finally, the 
	//			function can optionally save the resulting bitmap to a png file. 
	int PackFontRange(stbtt_fontinfo * STBFontInfo, stbtt_pack_range * range, int bitmap_width, int bitmap_height,
		bool saveToTexture,
		const char * textureFilename)
	{
		// output buffer containing the packed characters rectangles
		range->chardata_for_range = (stbtt_packedchar*)malloc(sizeof(stbtt_packedchar)* range->num_chars);

		// flag all characters as NOT packed
		for (int i = 0; i < range->num_chars; ++i)
			range->chardata_for_range[i].x0 =
			range->chardata_for_range[i].y0 =
			range->chardata_for_range[i].x1 =
			range->chardata_for_range[i].y1 = 0;

		// begin packing process
		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, NULL, bitmap_width, bitmap_height, 0, 1, NULL);
		stbtt_PackSetOversampling(&pc, PACK_OVERSAMPLE, PACK_OVERSAMPLE); // OPTIONAL

																		  // Alloc packed rectangles
		stbrp_rect    *rects;
		rects = (stbrp_rect *)std::malloc(sizeof(*rects) * range->num_chars);
		//DebugErrorIf(rects == NULL, "ERROR WHILE LOADING FONT"); // failure

																 // gather the number of rectangles from the ranges.
		int n = stbtt_PackFontRangesGatherRects(&pc, STBFontInfo, range, 1, rects);

		// pack them...
		stbtt_PackFontRangesPackRects(&pc, rects, n);

		// render if necessary to write to PNG file
		if (saveToTexture)
		{
			// bitmap to render to
			unsigned bmsize = sizeof(unsigned char)* bitmap_width * bitmap_height;
			pc.pixels = (unsigned char *)malloc(bmsize);
			std::memset(pc.pixels, 0, bmsize);

			// render packed font ranges
			stbtt_PackFontRangesRenderIntoRects(&pc, STBFontInfo, range, 1, rects);
			//DebugErrorIf(res == 0, "problem when packing font");
			stbi_write_png(textureFilename, bitmap_width, bitmap_height, 1, pc.pixels, 0);

			// free the pixels.
			free(pc.pixels);
		}

		// done packing
		stbtt_PackEnd(&pc);

		// success
		return 1;
	}

	void LoadTTF(const char * fileName,
		int start_char, int end_char,
		int bitmap_width, int bitmap_height,
		float font_size, FontInfo * outFontInfo,
		bool saveToTexture,
		const char * textureFilename)
	{
		// Sanity Checks
		//DebugAssert(fileName != NULL, "No Font file name provided");
		//DebugAssert(outFontInfo != NULL, "No Font Info provided when loading the font file");

		// Store the font info filename
		outFontInfo->mFontFileName = fileName;

		// no texture filename, just save to same dir and name
		if (textureFilename == NULL)
		{
			FilePath fp(fileName);
			outFontInfo->mFontBitmapFileName = fp.mDirectory + fp.mFilename + std::string(".png");
		}
		else
			outFontInfo->mFontBitmapFileName = textureFilename;

#pragma region LOAD FONT FILE

		// open font file and load ttf file in binary
		FILE * fontFilePtr = NULL;
		fopen_s(&fontFilePtr, fileName, "rb");

		// get how big is the file
		fseek(fontFilePtr, 0, SEEK_END);	// set the current position to end of file (note: this technique is not portable)
		int size = ftell(fontFilePtr);		// get the current position to get the size of the file.
		fseek(fontFilePtr, 0, SEEK_SET);	// reset the current position to the beginning.

											// buffer to hold the ttf file data
		unsigned char * ttf_buffer = (unsigned char*)malloc(size);
		fread(ttf_buffer, 1, size, fontFilePtr);

		// close the file
		fclose(fontFilePtr);

		// font info from stbtt
		stbtt_fontinfo STBFontInfo;

		// Initialize the font info structure
		int font_index = 0; // most files only have one font in them... this is here for completeness
		int font_start = stbtt_GetFontOffsetForIndex(ttf_buffer, font_index);
		stbtt_InitFont(&STBFontInfo, ttf_buffer, font_start);
#pragma endregion

#pragma region PACK FONTS AND SAVE FONT TEXTURE
		//
		// Pack the font data into a  bitmap
		stbtt_pack_range range;
		range.first_unicode_codepoint_in_range = start_char;
		range.array_of_unicode_codepoints = NULL;
		range.num_chars = end_char - start_char;
		range.font_size = font_size;
		PackFontRange(&STBFontInfo, &range, bitmap_width, bitmap_height, saveToTexture, outFontInfo->mFontBitmapFileName.c_str());

		outFontInfo->mFontBitmap = ResourceManager::textures["ProggyClean.png"].get();

#pragma endregion

#pragma region GENERATE FONTINFO GLYPH CONTAINER

		// Store basic information
		outFontInfo->mFirstCharacterCode = start_char;
		outFontInfo->mLastCharacterCode = end_char;
		outFontInfo->mCharacterCount = range.num_chars;
		outFontInfo->mFontSize = font_size;

		int num_char = range.num_chars;
		stbtt_packedchar * pdata = range.chardata_for_range;

		// allocate memory for the glyph and kerning table
		outFontInfo->mGlyphContainer = (Glyph *)(malloc(sizeof(Glyph)* num_char));
		outFontInfo->mKerningTable = (float**)(malloc(sizeof(float*)*num_char));
		for (int i = 0; i < num_char; ++i)
		{
			outFontInfo->mKerningTable[i] = (float *)(malloc(sizeof(float)*num_char));
			for (int j = 0; j < num_char; ++j)
				outFontInfo->mKerningTable[i][j] = 0.0f;
		}

		//
		// Compute font metrics and glyph data

		// Compute scale for a specific font_size. 
		float scale;
		scale = stbtt_ScaleForPixelHeight(&STBFontInfo, font_size);

		// compute line gap
		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(&STBFontInfo, &ascent, &descent, &lineGap);
		float ascentf = scale * (float)ascent;
		float descentf = scale * (float)descent;
		float lineGapf = scale * (float)lineGap;
		outFontInfo->mLineGap = ascentf - descentf + lineGapf;


		// 
		// Extract glyph specific metrics such as texture coordinates in bitmap from packed data and
		// bounding box of the glyph character

		// Retrieve the coordinates in the texture
		for (int char_index = start_char; char_index < end_char; ++char_index)
		{
			// get the index of the character in the array
			int array_index = char_index - start_char; // this is by the way how to retrieve it with AEX::FontInfo (TODO(Thomas) provide method to do this)

													   // get the glyph index from the font (this will increase performance according to STB_TT)
			int glyph_index = stbtt_FindGlyphIndex(&STBFontInfo, char_index);

			// get the glyph metrics
			int x0, y0, x1, y1; // unscaled glyph bounding box values
			stbtt_GetGlyphBox(&STBFontInfo, glyph_index, &x0, &y0, &x1, &y1);

			// Texture coodinates
			float xpos = 0.0f, ypos = 0.0f; // dummy values
			stbtt_aligned_quad aquad;
			stbtt_GetPackedQuad(pdata, bitmap_width, bitmap_height, array_index,
				&xpos, &ypos, &aquad, 0);

			// convert to positive y axis up
			aquad.t0 = 1.0f - aquad.t0;
			aquad.t1 = 1.0f - aquad.t1;
			float temp = aquad.t0;
			aquad.t0 = aquad.t1;
			aquad.t1 = temp;

			// store data into Glyph array
			// note: The glyph array stores the character in an array whose 
			// index doesn't relate to the character code in any way. 
			// use AEX::FontInfo to retrieve the glyph index based on a character
			// code.

			// Get the glyph
			Glyph & glyph = outFontInfo->mGlyphContainer[array_index];

			// store the character code
			glyph.mCharacterCode = char_index;

			// store the bounding box
			glyph.mX0 = scale * (float)x0;
			glyph.mX1 = scale * (float)x1;
			glyph.mY0 = scale * (float)y0;
			glyph.mY1 = scale * (float)y1;

			// store the texture coordinates
			glyph.mU0 = aquad.s0;
			glyph.mU1 = aquad.s1;
			glyph.mV0 = aquad.t0;
			glyph.mV1 = aquad.t1;

			// Get advance width
			int advanceWidth;
			stbtt_GetGlyphHMetrics(&STBFontInfo, glyph_index, &advanceWidth, 0); // last param is for left side bearing which is not used in our case
			glyph.mAdvanceWidth = scale * (float)advanceWidth;


			// get the kerning info for all the remaining characters. 
			for (int other_char_index = start_char;
				other_char_index < end_char;
				++other_char_index)
			{
				int other_glyph_index = stbtt_FindGlyphIndex(&STBFontInfo, other_char_index);
				int kern_advance = stbtt_GetGlyphKernAdvance(&STBFontInfo, glyph_index, other_glyph_index);
				int other_array_index = other_char_index - start_char;
				outFontInfo->mKerningTable[array_index][other_array_index] = scale * (float)kern_advance;
			}
		}
#pragma endregion

		// clean up
		free(range.chardata_for_range);
		free(ttf_buffer);
	}

}