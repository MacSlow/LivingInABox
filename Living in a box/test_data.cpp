#include "test_data.h"

using namespace std;
using namespace simd;


namespace { //anonymous namespace for local helper
    vector<double> testVector(int size)
    {
        const auto factor = 1.0 / RAND_MAX;
        auto v = vector<double>(size, 0.0);
        for (auto& x : v) x = rand() * factor;
        return v;
    }
    
    vector<double2> toVectorDouble2(const vector<double>& v)
    {
        vector<double2> v2;
        v2.reserve(v.size() / 2);
        for (size_t i = 0; i < v.size(); i += 2)
            v2.emplace_back(v[i], v[i+1]);
        return v2;
    }
    
    vector<double4> toVectorDouble4(const vector<double>& v)
    {
        vector<double4> v4;
        v4.reserve(v.size() / 4);
        for (size_t i = 0; i < v.size(); i += 4)
            v4.emplace_back(v[i], v[i+1], v[i+2], v[i+3]);
        return v4;
    }
}//end of anonymous namespace for local helper


vector<double> testVectorDouble(int i)
{
    //make sure the size can be divided by 4 (to be usable for simd2 and simd4)
    switch (i) {
    case 0 : return testVector( 60000000);
    case 1 : return testVector( 80000000);
    case 2 : return testVector(100000000);
    case 3 : return testVector(200000000);
    case 4 : return testVector(300000000);
    default: return testVector( 10000000);
    }
}

vector<double2> testVectorDouble2(int i)
{
    return toVectorDouble2(testVectorDouble(i));
}

vector<double4> testVectorDouble4(int i)
{
    return toVectorDouble4(testVectorDouble(i));
}
