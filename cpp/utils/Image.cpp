#include "Image.h"

#include <fstream>
#include <iostream>
#include <cassert>

namespace {
Pixel parsePixel(uint32_t inputRgba) {
  Pixel px;
  px.red = static_cast<uint8_t>( (inputRgba & 0xFF000000) >> 24);
  px.green = static_cast<uint8_t>( (inputRgba & 0x00FF0000) >> 16);
  px.blue = static_cast<uint8_t>( (inputRgba & 0x0000FF00) >> 8);
  px.alpha = static_cast<uint8_t>( (inputRgba & 0x000000FF));
  return px;
}
}

PackedImage::PackedImage(const Resolution &res)
    : resolution(res), pixels(res.width * res.height) {

}

bool PackedImage::operator==(const PackedImage &other) const {
  return (resolution == other.resolution) && (pixels == other.pixels);
}

std::istream& operator>>(std::istream &istream, PackedImage &img) {
  uint32_t pixelValue = 0;
  for (Pixel &pixel : img.pixels) {
    istream >> pixelValue;
    pixel = parsePixel(pixelValue);
  }

  return istream;
}

  Pixel & PackedImage::at(int32_t x, int32_t y)
  {
    return const_cast<Pixel&>(const_cast<PackedImage const *>(this)->at(x, y));
  }
  Pixel const & PackedImage::at(int32_t x, int32_t y) const
  {
    assert(x >= 0 && x < resolution.width && y >= 0 && y < resolution.height && "Invalid dimensions");
    return pixels[x + y * resolution.width];
  }

// ####################################################################################

StrideImage::StrideImage(const Resolution &res)
    : resolution(res), redPixels(res.width * res.height),
      greenPixels(res.width * res.height), bluePixels(res.width * res.height),
      alphaPixels(res.width * res.height) {

}

bool StrideImage::operator==(const StrideImage &other) const {
  return (resolution == other.resolution) &&
         (redPixels == other.redPixels) &&
         (greenPixels == other.greenPixels) &&
         (bluePixels == other.bluePixels) &&
         (alphaPixels == other.alphaPixels);
}

std::istream& operator>>(std::istream &istream, StrideImage &img) {
  const int32_t totalPixels = img.resolution.width * img.resolution.height;
  uint32_t pixelValue = 0;
  Pixel currPixel;
  for (int32_t i = 0; i < totalPixels; ++i) {
    istream >> pixelValue;
    currPixel = parsePixel(pixelValue);
    img.redPixels[i] = currPixel.red;
    img.greenPixels[i] = currPixel.green;
    img.bluePixels[i] = currPixel.blue;
    img.alphaPixels[i] = currPixel.alpha;
  }

  return istream;
}

void imwritePPM(std::string const & fname, PackedImage const & image)
{
  std::ofstream ofs(fname);
  ofs << "P6\n";
  ofs << image.resolution.width << " " << image.resolution.height << "\n";
  ofs << "255\n";
  for (auto const & p : image.pixels) {
    ofs.write((char*)&p, 3);
  }
}
