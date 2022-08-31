#include "Solution.h"

#include <cassert>
#include <cstdint>
#include <array>
#include <vector>
#include <cstring>
#include <algorithm>
#include <thread>

#include "utils/FunctionTracer.h"
#include "utils/EyePatterns.h"

#include "Queue.h"
#include "Task.h"

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
  for (const auto & index : indices) {
    if (image.at<0>(start.x + index.x, start.y + index.y) < redThreshold) {
      return false;
    }
  }
  return true;
}

inline void processImage(StrideImage & image, std::array<std::vector<PointI>, EYE_PATTERNS_COUNT> const & pattern, int ystart, int ysize)
{
  int yend = std::min(image.resolution.height - EYE_PATTERN_COL_SIZE + 1, ystart + ysize);
  for (int y = ystart; y < yend; y++) {
    for (int x = 0; x < image.resolution.width - EYE_PATTERN_COL_SIZE + 1; x++) {
      for (int p = 0; p < EYE_PATTERNS_COUNT; p++) {
        if (image.at<0>(x,y) >= redThreshold && isRedEye(image, {x,y}, pattern[p]))
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

  unsigned int numThreads = std::thread::hardware_concurrency();
  Queue<Task<StrideImage>> q;

  for (auto &image : images) {
    const int chunkSize = image.resolution.height/numThreads;
    const int remainder = image.resolution.height - (image.resolution.height/chunkSize)*chunkSize;
    int y;
    for(y = 0; y < image.resolution.height; y+=chunkSize) {
      q.push({&image, y, chunkSize});
    }
    q.push({&image, y, remainder});
  }

  // run threads only after queue of tasks is populated
  // this makes the queue simple which results in fast pop operation
  std::vector<std::thread> threadPool(numThreads);
  for (auto & thread : threadPool) {
    thread = std::thread([&](){
      Task<StrideImage> job;
      while(q.pop(job)) {
        processImage(*job.image, preprocessedPatterns, job.ystart, job.ysize);
      }
    });
  }

  for (auto & thread : threadPool) {
    if(thread.joinable()) {
      thread.join();
    }
  }
}

void Solution::compute([[maybe_unused]] std::vector<PackedImage> &images) {
  FunctionTracer<std::chrono::milliseconds> tracer("compute", "ms");
  // TODO: fill solution
}
