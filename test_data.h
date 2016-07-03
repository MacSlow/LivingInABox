#pragma once

#include <vector>
#include "simd_double2.h"
#include "simd_double4.h"
#include "allocator.h"

//For (small) int we return a big test vectors of random doubles. 
//For simd we take for the same i the same data but stored in registers.
std::vector<double>        testVectorDouble(int i);
std::vector<simd::double2> testVectorDouble2(int i);
std::vector<simd::double4, AlignedAllocator<simd::double4, 32>> testVectorDouble4(int i); 
