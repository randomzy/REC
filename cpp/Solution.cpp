#include "Solution.h"

#include <cassert>
#include <cstdint>
#include <array>
#include <vector>
#include <cstring>
#include <algorithm>

#include "utils/FunctionTracer.h"
#include "utils/EyePatterns.h"

constexpr uint8_t redThreshold = 200;
constexpr uint8_t redReduction = 150;

// takes a pattern and fills vector of (x,y) pairs, where each pair corresponds
// to non-whitespace symbol in pattern
static inline void nonWhitespaceIndices(EyePattern const & pattern, std::vector<PointI> & nonWhitespace)
{
  nonWhitespace.clear();
  for (int y = 0; y < EYE_PATTERN_COL_SIZE; y++) {
    const int row_len = std::strlen(pattern[y]);
    for (int x = 0; x < row_len; x++) {
      if (!std::isspace(pattern[y][x])) {
        nonWhitespace.push_back({x,y});
      }
    }
  }
}

static inline void correctRedEye(StrideImage & image, PointI const & start, std::vector<PointI> const & indices)
{
  for (auto const & index : indices) {
      image.at<0>(start.x + index.x, start.y + index.y) -= redReduction;
  }
}

static inline bool isRedEye(StrideImage const & image, PointI const & start, std::vector<PointI> const & indices)
{
  return std::all_of(indices.begin(), indices.end(), [&](auto const & p){
      return image.at<0>(start.x + p.x, start.y + p.y) >= redThreshold;
  });
}

inline void processImage(StrideImage & image, std::array<std::vector<PointI>, EYE_PATTERNS_COUNT> const & pattern)
{
  for (int y = 0; y < image.resolution.height - EYE_PATTERN_COL_SIZE + 1; y++) {
    for (int x = 0; x < image.resolution.width - EYE_PATTERN_COL_SIZE + 1; x++) {
      for (int p = 0; p < EYE_PATTERNS_COUNT; p++) {
        if (isRedEye(image, {x,y}, pattern[p]))
          correctRedEye(image, {x,y}, pattern[p]);
      }
    }
  }
}

void Solution::compute([[maybe_unused]] std::vector<StrideImage> &images) {
  FunctionTracer<std::chrono::milliseconds> tracer("compute", "ms");
  std::array<std::vector<PointI>, EYE_PATTERNS_COUNT> preprocessedPatterns;
  // note the reverse iterators when processing filters
  // order of filters is important
  // filtering [-] or [|] first when the case is [+] will result in skipped ..
  // or skipped : pixels
  std::transform(EYE_PATTERNS.rbegin(), EYE_PATTERNS.rend(), preprocessedPatterns.begin(),
                 [](auto const &pattern) {
                   std::vector<PointI> tmp;
                   nonWhitespaceIndices(pattern, tmp);
                   return tmp;
                 });
  for (auto &image : images)
    processImage(image, preprocessedPatterns);
}

void Solution::compute([[maybe_unused]] std::vector<PackedImage> &images) {
  FunctionTracer<std::chrono::milliseconds> tracer("compute", "ms");
  // TODO: fill solution
}
