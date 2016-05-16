#pragma once

#include <immintrin.h>

namespace simd {


struct double4
{
  //construct by register or by 4 double
  inline double4() {}
  inline double4(const __m256d& d4)                                                      : m_d4(d4)                            {}
  inline double4(const double& dA, const double& dB, const double& dC, const double& dD) : m_d4(_mm256_set_pd(dD, dC, dB, dA)) {}

  //register access
  inline       double& operator[](int i)       { return reinterpret_cast<      double*>(&m_d4)[i]; }
  inline const double& operator[](int i) const { return reinterpret_cast<const double*>(&m_d4)[i]; }

  //value in register
  __m256d m_d4;
};

//min max with fast SIMD operations
inline double4 min(const double4& d4A, double4& d4B) { return _mm256_min_pd(d4A.m_d4, d4B.m_d4); }
inline double4 max(const double4& d4A, double4& d4B) { return _mm256_max_pd(d4A.m_d4, d4B.m_d4); }
inline Box<double> make_double_box(const double4& mi, const double4& ma) 
{
    return make_box(
        std::min(std::min(mi[0], mi[1]), std::min(mi[2], mi[3])), 
        std::max(std::max(ma[0], ma[1]), std::max(ma[2], ma[3])));
}


}
