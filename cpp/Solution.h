#ifndef SOLUTION_H_
#define SOLUTION_H_

#include "utils/Image.h"

class Solution {
public:
  void compute([[maybe_unused]] std::vector<PackedImage> &images);

  void compute([[maybe_unused]] std::vector<StrideImage> &images);
};

#endif /* SOLUTION_H_ */
