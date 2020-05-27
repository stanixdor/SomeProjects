#include "ScreenCapture.hh"
#include "../Color/Color.hh"
#include "../Mouse/MouseUtilites.hh"
#include "../Keyboard/KeyboardUtilities.hh"

#include <cmath>
#include <algorithm>
#include <string>




ScreenCapture::ScreenCapture()
{
	ScreenX = GetDeviceCaps(GetDC(0), HORZRES);
	ScreenY = GetDeviceCaps(GetDC(0), VERTRES);
	ScreenData = new BYTE[3 * ScreenX * ScreenY];

	for (int i = 0; i < 10; ++i)
		dataNumbers.push_back(bitmap_image(std::to_string(i) + ".bmp"));

	for (int i = 1; i <= 18; ++i)
		dataLevels.push_back(bitmap_image("lvl" + std::to_string(i) + ".bmp"));
}

void ScreenCapture::ScreenCap()
{
	HDC hdc = GetDC(NULL), hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, ScreenX, ScreenY);
	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 24;
	bmi.biWidth = ScreenX;
	bmi.biHeight = -static_cast<int>(ScreenY);
	bmi.biCompression = BI_RGB;
	SelectObject(hdcMem, hBitmap);
	BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hdc, 0, 0, SRCCOPY);

	GetDIBits(hdc, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdc);
}

void ScreenCapture::ProcessImageBinary(bitmap_image& image, const bool invert, const unsigned char treshold) const
{
	if(invert)
		image.invert_color_planes();

	image.convert_to_grayscale();

	for (unsigned y = 0; y < image.height(); ++y)
		for (unsigned x = 0; x < image.width(); ++x)
		{
			unsigned char r = image.red_channel(x, y);
			
			r = r < treshold ? r = 0 : r = 255;
			image.set_pixel(x, y, r, r, r);
			
		}
}
void ScreenCapture::ApplyConvolution(bitmap_image& image, const float* kernel, const unsigned iterations ) const
{

	int kw = 3;
	int kh = 3;

	int offsetX = (kw - 1) / 2;
	int offsetY = (kh - 1) / 2;

	for (unsigned iter = 0; iter < iterations; ++iter)
	{
		for (unsigned y = 0; y < image.height(); y++)
		{

			//rgb_t* outLine1 = (rgb_t*)image.scanLine(y);

			for (unsigned x = 0; x < image.width(); x++)
			{
				float pixelR = 0;
				float pixelG = 0;
				float pixelB = 0;

				// Apply convolution to each channel.
				for (int j = 0; j < kh; j++)
				{
					if (y + j < offsetY || y + j - offsetY >= image.height())
						continue;

					//const rgb_t* inLine = (rgb_t*)image.constScanLine(y + j - offsetY);

					for (int i = 0; i < kw; i++)
					{
						if (x + i < offsetX || x + i - offsetX >= image.width())
							continue;

						float k = kernel[i + j * kw];
						rgb_t pixel = image.get_pixel(x + i - offsetX, y + j - offsetY);
						//rgb_t pixel = inLine[x + i - offsetX];

						pixelR += k * pixel.red;
						pixelG += k * pixel.green;
						pixelB += k * pixel.blue;
					}
				}

				unsigned char r = std::clamp(pixelR, 0.f, 255.f);
				unsigned char g = std::clamp(pixelG, 0.f, 255.f);
				unsigned char b = std::clamp(pixelB, 0.f, 255.f);

				image.set_pixel(x, y, r, g, b);
				//outLine[x] = qRgb(r, g, b);
			}
		}
	}
	

}

unsigned ScreenCapture::NumberOfDigits(const bitmap_image& image) const
{
	//TAKING INTO ACCOUNT IMAGE WAS CONVERTED TO GREY SCALE AND BINARY-ED

	unsigned black = 0;
	unsigned posY = image.height() / 4;
	unsigned firstCharPos = image.width();


	for (unsigned x = 0; x < image.width(); ++x)
	{
		if (image.red_channel(x, posY) != black)
		{
			firstCharPos = x;
			break;
		}
	}

	//std::cout << firstCharPos << "\n"; 

	if (firstCharPos < 9)
	{
		return 5;
	}
	else if (firstCharPos < 12)
	{
		return 4;
	}
	else if (firstCharPos <= 15)
	{
		return 3;
	}
	else if (firstCharPos < 19)
	{
		return 2;
	}
	else if (firstCharPos < 24)
	{
		return 1;
	}
	else return 4;


	return 0;

}

int ScreenCapture::GuessNumber(const bitmap_image& image, const unsigned numberOfDigits, const bool baron) const
{
	//std::cout << numberOfDigits << "\n";
	unsigned startX;
	unsigned startY = 1;
	unsigned endY = image.height() - startY;

	switch (numberOfDigits)
	{
	case 1:
		{
		startX = 19;
		break;
		}
	case 2:
	{
		startX = 15;
		break;
	}
	case 3:
	{
		if(baron)
			startX = 12;
		else startX = 11;
		break;
	}
	case 4:
	{
		startX = 8;
		break;
	}
	default:
	{
		startX = image.width() / 2; //crefull out of memory acces
		break;
	}
	}

	unsigned endX = image.width() - startX;
	
	unsigned distX = endX - startX;
	unsigned distY = endY - startY;

	unsigned partition = distX / numberOfDigits;

	std::vector<bitmap_image> imgV;
	imgV.reserve(numberOfDigits);
	imgV.resize(numberOfDigits);


	for (unsigned i = 0; i < numberOfDigits; ++i)
	{
		imgV[i].setwidth_height(partition, distY);
		for (unsigned y = startY, yy = 0; y < endY; ++y, ++yy)
			for (unsigned x = startX + i * partition, xx = 0; x < startX + (i * partition + partition); ++x, ++xx)
			{
				imgV[i].set_pixel(xx, yy, image.get_pixel(x, y));
			}
		//imgV[i].save_image("temp" + std::to_string(i) + ".bmp");
	}
	unsigned finalNumber = 0;
	unsigned indexSum = 1;
	for (unsigned i = 0; i < numberOfDigits-1; ++i, indexSum *= 10) {}

	for (unsigned i = 0; i < numberOfDigits; ++i)
	{
		float smallestDiff = 100.f;
		unsigned deltaIndex = 0;

		for (unsigned j = 0; j < 10; ++j)
		{
			double totaldiff = 0.0; //holds the number of different pixels
			int h = imgV[i].height();
			int w = imgV[i].width();
			int hsecond = dataNumbers[j].height();
			int wsecond = dataNumbers[j].width();
			/*if (w != wsecond || h != hsecond) 
			{
				std::cerr << "Error, pictures must have identical dimensions!\n";
				return 2;
			}*/
			for (int y = 0; y < h; y++) 
			{
				for (int x = 0; x < w; x++) 
				{
					unsigned pixelFirst = imgV[i].red_channel(x, y);
					int rFirst = pixelFirst;
					unsigned pixelSecond = dataNumbers[j].red_channel(x, y);
					int rSecond = pixelSecond;
					totaldiff += std::abs(rFirst - rSecond) / 255.0;
				}
			}
			deltaIndex = totaldiff < smallestDiff ? j : deltaIndex;
			smallestDiff = totaldiff < smallestDiff ? totaldiff : smallestDiff;
		}
		if (smallestDiff > 2.f)
		{
			return 999999;
		}
		finalNumber += deltaIndex * indexSum;
		
		indexSum /= 10;
	}
	return finalNumber;


}

unsigned ScreenCapture::GetChampionLevel() const
{
	const bitmap_image image = ScreenRegionToImage(707, 1050, 722, 1064, false);

	float smallestDiff = 100.f;
	unsigned deltaIndex = 0;

	for (unsigned i = 0; i < 18; ++i)
	{
		double totaldiff = 0.0; //holds the number of different pixels
		int h = image.height();
		int w = image.width();
		int hsecond = dataLevels[i].height();
		int wsecond = dataLevels[i].width();
		if (w != wsecond || h != hsecond)
		{
			std::cerr << "Error, pictures must have identical dimensions!\n";
			return 2;
		}
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				rgb_t pixelFirst = image.get_pixel(x, y);
				int rFirst = pixelFirst.red;
				int gFirst = pixelFirst.green;
				int bFirst = pixelFirst.blue;
				rgb_t pixelSecond = dataLevels[i].get_pixel(x, y);
				int rSecond = pixelSecond.red;
				int gSecond = pixelSecond.green;
				int bSecond = pixelSecond.blue;
				totaldiff += std::abs(rFirst - rSecond) / 255.0;
				totaldiff += std::abs(gFirst - gSecond) / 255.0;
				totaldiff += std::abs(bFirst - bSecond) / 255.0;
			}
		}
		deltaIndex = totaldiff < smallestDiff ? i : deltaIndex;
		smallestDiff = totaldiff < smallestDiff ? totaldiff : smallestDiff;
	}

	return deltaIndex+1;
}

void ScreenCapture::UseSmite() const
{

	Position pos, posBaron;
	Color posColor, posColorBaron;

	posColor = Color(145, 126, 74);
	posColorBaron = Color(151, 134, 85);


	unsigned width = ScreenWidth();
	unsigned height = ScreenHeight();
	bool found = false;
	bool foundBaron = false;

	for (unsigned y = 0; y < height; ++y)
		for (unsigned x = 0; x < width; ++x)
		{
			if (posColor == GetColor(x, y))
			{
				pos.x = x;
				pos.y = y;

				int offx = -4;
				int offy = 19;
				int posToCheckX = pos.x + offx;
				int posToCheckY = pos.y + offy;
				Color cToCheck1{ 148, 121, 66 };
				Color cToCheck2{ 112, 92, 46 };
				posToCheckX = std::clamp(posToCheckX, 0, static_cast<int>(width));
				posToCheckY = std::clamp(posToCheckY, 0, static_cast<int>(height));

				Color temp = GetColor(posToCheckX, posToCheckY);

				if (cToCheck1 == temp || cToCheck2 == temp)
				{
					found = true;
				}


			}
			else if (posColorBaron == GetColor(x, y))
			{
				posBaron.x = x;
				posBaron.y = y;

				int offx = -1;
				int offy = 23;
				int posToCheckX = posBaron.x + offx;
				int posToCheckY = posBaron.y + offy;
				Color cToCheck1{ 198, 166, 99 };
				posToCheckX = std::clamp(posToCheckX, 0, static_cast<int>(width));
				posToCheckY = std::clamp(posToCheckY, 0, static_cast<int>(height));

				Color temp = GetColor(posToCheckX, posToCheckY);

				if (cToCheck1 == temp /*|| cToCheck2 == temp*/)
				{
					foundBaron = true;
					
				}
			}
		}
	if (foundBaron)
	{
		int offsetXstart = -22;
		int offsetYstart = -15;

		int offsetXend = 23;
		int offsetYend = 0;

		int posXstart = static_cast<int>(posBaron.x) + offsetXstart;
		if (posXstart < 0)
			posXstart = 0;
		int posYstart = static_cast<int>(posBaron.y) + offsetYstart;
		if (posYstart < 0)
			posYstart = 0;

		int posXend = static_cast<int>(posBaron.x) + offsetXend;
		if (posXend > width)
			posXend = width;
		int posYend = static_cast<int>(posBaron.y) + offsetYend;
		if (posYend > height)
			posYend = height;

		bitmap_image img = ScreenRegionToImage(posXstart, posYstart, posXend, posYend, true, 0);
		for (unsigned x = 0; x < img.width(); ++x)
		{
			img.set_pixel(x, img.height() - 1, 0, 0, 0);
		}
		//img.save_image("testbaron.bmp");
		unsigned numberOfDigits = NumberOfDigits(img);
		
		if (numberOfDigits > 4)
		{
			return;
		}
		else
		{
			unsigned targetHP = GuessNumber(img, numberOfDigits, true);
			if (targetHP <= smiteDMGperLevel[GetChampionLevel() - 1])
			{
				Position smitingPos(posBaron.x, std::clamp(posBaron.y + 120, 0, 1080));
				MouseUtilities::mouseMoveTo(smitingPos);
				Sleep(20);
				KeyboardUtilities::PressKey('F');
			}
		}


	}
	else if (found)
	{
		int offsetXstart = -26;
		int offsetYstart = -17;

		int offsetXend = 19;
		int offsetYend = -2;

		int posXstart = static_cast<int>(pos.x) + offsetXstart;
		if (posXstart < 0)
			posXstart = 0;
		int posYstart = static_cast<int>(pos.y) + offsetYstart;
		if (posYstart < 0)
			posYstart = 0;

		int posXend = static_cast<int>(pos.x) + offsetXend;
		if (posXend > width)
			posXend = width;
		int posYend = static_cast<int>(pos.y) + offsetYend;
		if (posYend > height)
			posYend = height;

		bitmap_image img = ScreenRegionToImage(posXstart, posYstart, posXend, posYend, true, 0);

		//img.save_image("test.bmp");
		unsigned numberOfDigits = NumberOfDigits(img);
		unsigned targetHP = GuessNumber(img, numberOfDigits,false);

		if (numberOfDigits > 4)
		{
			return;
		}
		else if (targetHP < smiteDMGperLevel[GetChampionLevel() - 1])
		{
			std::cout << numberOfDigits << " " << targetHP << "\n";
			Position smitingPos(pos.x, std::clamp(pos.y + 100, 0, 1080));
			
			MouseUtilities::mouseMoveTo(smitingPos);
			Sleep(20);
			KeyboardUtilities::PressKey('F');
		}

	}

}

bitmap_image ScreenCapture::ScreenRegionToImage(const int x1, const int y1, const int x2, const int y2, const bool imageProcessing, const unsigned upscaleFactor) const
{
	unsigned width = abs(x1 - x2);
	unsigned height = abs(y1 - y2);
	bitmap_image image(width, height);

	unsigned xStart = x1 < x2 ? x1 : x2;
	unsigned yStart = y1 < y2 ? y1 : y2;

	for (unsigned int x = 0; x < width; ++x)
	{
		for (unsigned int y = 0; y < height; ++y)
		{
			rgb_t col = rgb_t();
			col.red = GetR(xStart + x, yStart + y);
			col.green = GetG(xStart + x, yStart + y);
			col.blue = GetB(xStart + x, yStart + y);


			image.set_pixel(x, y, col.red, col.green, col.blue);
		}
	}
	float GaussianKernel[] = { 0.109634f, 0.111842f, 0.109634f,
							 0.111842f, 0.114094f, 0.111842f,
							 0.109634f, 0.111842f, 0.109634f };
	float BoxBlurKernel[] = { 1.f/9.f, 1.f / 9.f, 1.f / 9.f,
							 1.f / 9.f, 1.f / 9.f, 1.f / 9.f,
							 1.f / 9.f, 1.f / 9.f, 1.f / 9.f };

	float SharpenKernel[] = { -1, -1, -1,
							 -1,  9, -1,
							 -1, -1, -1 };
	if (upscaleFactor == 0)
	{
		if (imageProcessing)
		{
			ProcessImageBinary(image, false, 140);
			//ApplyConvolution(image, GaussianKernel, 4);
			//ApplyConvolution(image, BoxBlurKernel,3);
		}
		return image;
	}
	else
	{
		std::vector<bitmap_image> upsamples;
		upsamples.reserve(upscaleFactor);
		upsamples.resize(upscaleFactor);
		for (size_t i = 0; i < upsamples.size(); ++i)
		{
			//special case
			if (i == 0)
				image.upsample(upsamples[i]);
			else
				upsamples[i - 1].upsample(upsamples[i]);
		}

		if (imageProcessing)
		{
			ProcessImageBinary(upsamples.back(), false, 140);
			//ApplyConvolution(upsamples.back(), GaussianKernel, 4);
			//ApplyConvolution(upsamples.back(), BoxBlurKernel, 6);
		}
		return upsamples.back();
	}
	
}

inline unsigned ScreenCapture::GetR(const unsigned x, const unsigned y) const
{
	return ScreenData[3 * ((y * ScreenX) + x) + 2];
}

inline unsigned ScreenCapture::GetG(const unsigned x, const unsigned y) const
{
	return ScreenData[3 * ((y * ScreenX) + x) + 1];
}

inline unsigned ScreenCapture::GetB(const unsigned x, const unsigned y) const
{
	return ScreenData[3 * ((y * ScreenX) + x)];
}

Color ScreenCapture::GetColor(const unsigned x, const unsigned y) const
{
	return Color{ GetR(x,y),GetG(x,y), GetB(x,y) };
}


ScreenCapture::~ScreenCapture()
{
	delete[] ScreenData;
}
