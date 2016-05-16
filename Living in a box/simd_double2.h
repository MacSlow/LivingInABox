#pragma once

#include <array>
#include <emmintrin.h>
#include "box.h"

namespace simd {


struct double2
{
  //construct by register or by 2 double
  inline double2(const __m128d& d2)                  : m_d2(d2) {}
  inline double2(const double& dA, const double& dB) : m_d2(_mm_set_pd(dB, dA)) {}

  //register access
  inline       double& operator[](int i)       { return reinterpret_cast<      double*>(&m_d2)[i]; }
  inline const double& operator[](int i) const { return reinterpret_cast<const double*>(&m_d2)[i]; }

  //value in register
  __m128d m_d2;
};

//min max with fast SIMD operations
inline double2 min(const double2& d2A, double2& d2B) { return _mm_min_pd(d2A.m_d2, d2B.m_d2); }
inline double2 max(const double2& d2A, double2& d2B) { return _mm_max_pd(d2A.m_d2, d2B.m_d2); }
inline Box<double> make_double_box(const double2& mi, const double2& ma) { return make_box(std::min(mi[0], mi[1]), std::max(ma[0], ma[1])); }



struct Box2d
{
    inline Box2d() {}
    inline Box2d(const Box2d&   b2) { m_minmax = b2.m_minmax; }
    inline Box2d(const __m128d& d2) { m_minmax[0] = d2; m_minmax[1] = d2; }
    inline Box2d(const double2& d2) { m_minmax[0] = d2.m_d2; m_minmax[1] = d2.m_d2; }

    inline double min() const { return std::min(get(0, 0), get(0, 1)); } 
    inline double max() const { return std::max(get(1, 0), get(1, 1)); }
    
    std::array<__m128d, 2> m_minmax;
 
    inline       double& get(int i, int j)       { return reinterpret_cast<      double*>(&m_minmax[i])[j]; }
    inline const double& get(int i, int j) const { return reinterpret_cast<const double*>(&m_minmax[i])[j]; }
};

inline Box2d& insertValue(Box2d& box, const double2& d)
{
    box.m_minmax[0] = _mm_min_pd(d.m_d2, box.m_minmax[0]);
    box.m_minmax[1] = _mm_max_pd(d.m_d2, box.m_minmax[1]);
    return box;
}
inline Box2d& insertBox(Box2d& box, const Box2d& b)
{
    box.m_minmax[0] = _mm_min_pd(b.m_minmax[0], box.m_minmax[0]);
    box.m_minmax[1] = _mm_max_pd(b.m_minmax[1], box.m_minmax[1]);
    return box;
}


}
