#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <vector>
#include <iosfwd>
#include <cassert>
#include <array>

#include "CommonDefines.h"

struct PackedImage {
  PackedImage(const Resolution& res);

  bool operator==(const PackedImage& other) const;
  friend std::istream& operator>>(std::istream& istream, PackedImage& img);

  Resolution resolution;
  std::vector<Pixel> pixels;

  Pixel & at(int32_t x, int32_t y);
  Pixel const & at(int32_t x, int32_t y) const;
};

struct StrideImage {
  StrideImage(const Resolution& res);

  bool operator==(const StrideImage& other) const;
  friend std::istream& operator>>(std::istream& istream, StrideImage& img);

  Resolution resolution;
  std::vector<uint8_t> redPixels;
  std::vector<uint8_t> greenPixels;
  std::vector<uint8_t> bluePixels;
  std::vector<uint8_t> alphaPixels;

  // compiler cannot inline explicit instantiated function
  // so define it in header
  template<int Channel>
  uint8_t & at(int32_t x, int32_t y)
  {
    return const_cast<uint8_t&>(const_cast<StrideImage const *>(this)->at<Channel>(x,y));
  }
  template<int Channel>
  uint8_t const & at(int32_t x, int32_t y) const
  {
    assert(x >= 0 && x < resolution.width && y >= 0 && y < resolution.height && "Invalid dimensions");
    constexpr std::array<const std::vector<uint8_t> StrideImage::*, 4> channels = {
      &StrideImage::redPixels,
      &StrideImage::greenPixels,
      &StrideImage::bluePixels,
      &StrideImage::alphaPixels
    };
    auto ch = channels[Channel];
    return (this->*ch)[x + y * resolution.width];
  }
};

void imwritePPM(std::string const & fname, PackedImage const &);

#endif /* IMAGE_H */
