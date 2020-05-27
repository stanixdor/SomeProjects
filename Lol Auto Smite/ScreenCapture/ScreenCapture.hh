#pragma once
#include "../BMP/bitmap_image.hh"
#include <Windows.h>
#include <vector>
#include <array>

struct Color;

class ScreenCapture
{
private:
    ScreenCapture(); // Disallow instantiation outside of the class.

    unsigned ScreenX;
    unsigned ScreenY;
    BYTE* ScreenData = nullptr;
    std::vector<bitmap_image> dataNumbers;
    std::vector<bitmap_image> dataLevels;
    std::array<unsigned, 18> smiteDMGperLevel{390,410,430,450,480,510,540,570,600,640,680,720,760,800,850,900,950,1000};

public:

    ScreenCapture(const ScreenCapture&) = delete;
    ScreenCapture& operator=(const ScreenCapture&) = delete;
    ScreenCapture(ScreenCapture&&) = delete;
    ScreenCapture& operator=(ScreenCapture&&) = delete;
    ~ScreenCapture();

    void ScreenCap();
    bitmap_image ScreenRegionToImage(const int x1, const int y1, const int x2, const int y2, bool imageProcessing = false, const unsigned upscaleFactor = 0) const;
    unsigned GetR(const unsigned x, const unsigned y) const;
    unsigned GetG(const unsigned x, const unsigned y) const;
    unsigned GetB(const unsigned x, const unsigned y) const;
    Color GetColor(const unsigned x, const unsigned y) const;

    unsigned ScreenWidth() const { return ScreenX; }
    unsigned ScreenHeight() const { return ScreenY; }

    void ProcessImageBinary(bitmap_image& image, const bool invert = false, const unsigned char treshold = 180) const;
    void ApplyConvolution(bitmap_image& image, const float* kernel, const unsigned iterations = 1) const;

    unsigned NumberOfDigits(const bitmap_image& image) const;

    int GuessNumber(const bitmap_image& image, const unsigned numberOfDigits, const bool baron) const;

    unsigned GetChampionLevel() const;

    void UseSmite() const;

    static ScreenCapture& Instance() 
    {
        static ScreenCapture screenCapture;
        return screenCapture;
    }

};