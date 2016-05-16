#pragma once;

#include <algorithm>
#include <future>
#include <numeric>
#include <thread>
#include <tuple>
#include "box.h"
#include "simd_double2.h"
#include "simd_double4.h"

using namespace std; //sorry for this "using" in global namespace 

namespace for_for 
{
    double min(const vector<double>& v)
    {
        double mi = v.front();
        for (auto d : v) 
            if (d < mi) mi = d;
        return mi;
    }
    double max(const vector<double>& v)
    {
        double ma = v.front();
        for (auto d : v) 
            if (d > ma) ma = d;
        return ma;
    }
    Box<double> calcBox(const vector<double>& v)
    {
        return make_box(min(v), max(v));
    }
}
namespace for_min_max 
{
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            mi = min(mi, d);
            ma = max(ma, d); 
        }
        return make_box(mi, ma);
    }
}
namespace for_own_min_max 
{
    inline double myMin(double left, double right) { return right < left ? right : left; }
    inline double myMax(double left, double right) { return left < right ? right : left; }

    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            mi = myMin(mi, d);
            ma = myMax(ma, d); 
        }
        return make_box(mi, ma);
    }
}
namespace for_lower_if_if 
{
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0; i < v.size(); ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_lower_if_else 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0; i < v.size(); ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            else if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_if_if 
{
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_if_else 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            else if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_if_continue 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            if (mi <= d && d <= ma) continue;
            mi = min(d, mi);
            ma = max(d, ma); 
        }
        return make_box(mi, ma);
    }
}
namespace for_if_continue_nan_check 
{    
    //v has only NaNs <=> min and max are both NaN
    //v has a real item <=> min and max are both real
    Box<double> calcBoxWithCheck(const vector<double>& v, size_t* pCounterNaN)
    {
        auto mi = v.front();
        auto ma = v.front();

        size_t nans = mi == mi ? 0 : 1;

        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];

            //Minimize comparisions for "typical" data:
            //often d is already between min and max
            if (mi <= d && d <= ma) continue;

            //sometime d is a new min or max
            if (d < mi) { mi = d; continue; }
            if (ma < d) { ma = d; continue; }

            //rarely d is NaN and all comparision above are false
            if (!(d == d)) {++nans; continue; }

            //rarely d is not NaN but all items before
            mi = ma = d;
        }

        if (pCounterNaN) *pCounterNaN += nans;
        return make_box(mi, ma);
    }

    Box<double> calcBox(const vector<double>& v)
    {
        return calcBoxWithCheck(v, nullptr);
    }

}
namespace for_conditional 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            d < mi ? mi = d : ma < d ? ma = d : d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_fixend_if_if 
{
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0, end = v.size(); i != end; ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_fixend_if_else 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0, end = v.size(); i != end; ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            else if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_fixend_if_continue 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0, end = v.size(); i != end; ++i) {
            const auto& d = v[i];
            if (mi <= d && d <= ma) continue;
            mi = min(d, mi);
            ma = max(d, ma); 
        }
        return make_box(mi, ma);
    }
}
namespace for_fixend_conditional 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (size_t i = 0, end = v.size(); i != end; ++i) {
            const auto& d = v[i];
            d < mi ? mi = d : ma < d ? ma = d : d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_if_if_pragma_no_vector
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        #pragma loop( no_vector )
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}

namespace for_if_else_pragma_no_vector 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        #pragma loop( no_vector )
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            else if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_if_continue_pragma_no_vector 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        #pragma loop( no_vector )
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            if (mi <= d && d <= ma) continue;
            mi = min(d, mi);
            ma = max(d, ma); 
        }
        return make_box(mi, ma);
    }
}
namespace for_conditional_pragma_no_vector 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        #pragma loop( no_vector )
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            d < mi ? mi = d : ma < d ? ma = d : d; 
        }
        return make_box(mi, ma);
    }
}
namespace for_if_if_pragma_hint_parallel
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        #pragma loop( hint_parallel(0) )
        for (size_t i = 0; i != v.size(); ++i) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}

namespace range_for_if_if 
{
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) {
            if (d < mi) mi = d;
            if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace range_for_min_max 
{
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) {
            mi = min(mi, d);
            ma = max(ma, d); 
        }
        return make_box(mi, ma);
    }
}
namespace range_for_if_else 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) {
            if (d < mi) mi = d;
            else if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace range_for_if_continue 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) {
            if (mi <= d && d <= ma) continue;
            mi = min(d, mi);
            ma = max(d, ma); 
        }
        return make_box(mi, ma);
    }
}

namespace range_for_conditional 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) 
            d < mi ? mi = d : ma < d ? ma = d : d; 
        return make_box(mi, ma);
    }
}
namespace range_for_conditional_conditional 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) {
            mi = d < mi ? d : mi;
            ma = ma < d ? d : ma;
        }
        return make_box(mi, ma);
    }
}
namespace range_for_conditional_conditional_reverse 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) {
            mi = mi <= d ? mi : d;
            ma = d <= ma ? ma : d;
        }
        return make_box(mi, ma);
    }
}
namespace range_for_operator 
{
    struct B
    {
        double mi;
        double ma;
        void operator()(double d) 
        {
            if (d < mi) mi = d;
            if (ma < d) ma = d;
        }
    };

    Box<double> calcBox(const vector<double>& v)
    {
        B b;
        b.mi = v.front();
        b.ma = v.front();
        for (const auto& d : v) b(d);
        return make_box(b.mi, b.ma);
    }
}

namespace while_if_if 
{
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        size_t i = 0;
        size_t end = v.size();
        while (i++ != end) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace while_if_else 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        size_t i = 0;
        size_t end = v.size();
        while (i++ != end) {
            const auto& d = v[i];
            if (d < mi) mi = d;
            else if (ma < d) ma = d; 
        }
        return make_box(mi, ma);
    }
}
namespace while_if_continue 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto mi = v.front();
        auto ma = v.front();
        size_t i = 0;
        size_t end = v.size();
        while (i++ != end) {
            const auto& d = v[i];
            if (mi <= d && d <= ma) continue;
            mi = min(d, mi);
            ma = max(d, ma); 
        }
        return make_box(mi, ma);
    }
}
namespace while_conditional 
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto min = v.front();
        auto max = v.front();
        size_t i = 0;
        size_t end = v.size();
        while (i++ != end) {
            const auto& d = v[i];
            d < min ? min = d : max < d ? max = d : d; 
        }
        return make_box(min, max);
    }
}

namespace std_min_element_max_element
{    
    Box<double> calcBox(const vector<double>& v)
    {
        return make_box(*min_element(begin(v),end(v)), *max_element(begin(v), end(v)));
    }
}
namespace std_minmax_element
{    
    Box<double> calcBox(const vector<double>& v)
    {
        const auto p = minmax_element(begin(v), end(v));
        return make_box(*p.first, *p.second);
    }
}

namespace std_accumulate_function
{    
    inline Box<double>& insert(Box<double>& b, double d)
    {
        b.min = d < b.min ? d : b.min;
        b.max = b.max < d ? d : b.max;
        return b;
    }

    Box<double> calcBox(const vector<double>& v)
    {
        return accumulate(begin(v), end(v), make_box(v.front()), insert);
    }
}
namespace std_accumulate_lambda
{    
    Box<double> calcBox(const vector<double>& v)
    {
        return accumulate(begin(v), end(v), make_box(v.front()), [](Box<double>& b, double d)
            { 
                b.min = d < b.min ? d : b.min;
                b.max = b.max < d ? d : b.max;
                return b;                
            });
    }
}
namespace std_accumulate_lambda_return
{    
    Box<double> calcBox(const vector<double>& v)
    {
        return accumulate(begin(v), end(v), make_box(v.front()), [](Box<double>& b, double d)
            { 
                if (b.min <= d && d < b.max) 
                    return b;

                d < b.min ? b.min = d : b.max = d;
                return b;                
            });
    }
}
namespace std_accumulate_lambda_copy
{    
    Box<double> calcBox(const vector<double>& v)
    {
        return accumulate(begin(v), end(v), make_box(v.front()), [](const Box<double>& b, double d)
            { 
                Box<double> c;
                c.min = d < b.min ? d : b.min;
                c.max = b.max < d ? d : b.max;
                return c;                
            });
    }
}

namespace std_future_accumulate_insert
{    
    inline Box<double>& insertValue(Box<double>& b, double d)
    {
        b.min = d < b.min ? d : b.min;
        b.max = b.max < d ? d : b.max;
        return b;
    }

    inline Box<double>& insertFutureBox(Box<double>& b, future<Box<double>>& futureBox)
    {
        auto c = futureBox.get();
        b.min = c.min < b.min ? c.min : b.min;
        b.max = b.max < c.max ? c.max : b.max;
        return b;
    }

    Box<double> calcBox(const vector<double>& v)
    {
        auto parts = thread::hardware_concurrency();
        auto size  = v.size();
        auto first = begin(v);

        vector<future<Box<double>>> futures;
        for (size_t i = 0; i != parts; ++i) {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [first, part_size] 
                { 
                    return accumulate(first, next(first, part_size), make_box(*first), insertValue); 
                }));
            advance(first, part_size);
        }

        return accumulate(begin(futures), end(futures), make_box(v.front()), insertFutureBox);
    }
}
namespace std_future_accumulate_lambda
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto parts = thread::hardware_concurrency();
        auto size  = v.size();
        auto first = begin(v);

        vector<future<Box<double>>> futures;
        for (size_t i = 0; i != parts; ++i) {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [first, part_size] 
                { 
                    return accumulate(first, next(first, part_size), make_box(*first), [](Box<double>& b, double d)
                    {
                        b.min = d < b.min ? d : b.min;
                        b.max = b.max < d ? d : b.max;
                        return b;
                    }); 
                }));
            advance(first, part_size);
        }

        return accumulate(begin(futures), end(futures), make_box(v.front()), [](Box<double>& b, future<Box<double>>& futureBox)
            {
                auto c = futureBox.get();
                b.min = c.min < b.min ? c.min : b.min;
                b.max = b.max < c.max ? c.max : b.max;
                return b;
            });
    }
}
namespace std_future_while
{    
    Box<double> calcBox(const vector<double>& v)
    {
        auto parts = thread::hardware_concurrency();
        auto size = v.size();
        size_t first = 0;

        vector<future<Box<double>>> futures;
        for (size_t i = 0; i != parts; ++i) 
        {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [&v, first, part_size]
            { 
                auto mi = v[first];
                auto ma = mi;
                size_t i = first;
                size_t end = first + part_size;
                while (i++ != end) {
                    const double& d = v[i];
                    mi = d < mi ? d : mi;
                    ma = ma < d ? d : ma;
                }
                return make_box(mi, ma);
            }));
            first += part_size;
        }

        auto mi = v.front();
        auto ma = v.front();
        for (auto& fut : futures) {
            auto c = fut.get();
            mi = c.min < mi ? c.min : mi;
            ma = ma < c.max ? c.max : ma;
        }
        return make_box(mi, ma);
    }
}
namespace accumulate_parallel
{    
    template<class Container, class Result, class PartFunc, class Func>
    Result accumulate_parallel(const Container& v, Result init, PartFunc partFunc, Func func)
    {
        auto parts = thread::hardware_concurrency();
        auto size  = v.size();
        auto first = begin(v);

        vector<future<Result>> futures;
        for (size_t i = 0; i != parts; ++i) {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [init, partFunc, first, part_size] 
                { 
                    return accumulate(first, next(first, part_size), init, partFunc); 
                }));
            advance(first, part_size);
        }

        return accumulate(begin(futures), end(futures), init, [func](Result& prevResult, future<Result>& fut)
            { 
                return func(prevResult, fut.get()); 
            });
    }

    inline Box<double>& insertValue(Box<double>& b, double d)
    {
        b.min = d < b.min ? d : b.min;
        b.max = b.max < d ? d : b.max;
        return b;
    }

    inline Box<double>& insertBox(Box<double>& b, const Box<double>& c)
    {
        b.min = c.min < b.min ? c.min : b.min;
        b.max = b.max < c.max ? c.max : b.max;
        return b;
    }

    Box<double> calcBox(const vector<double>& v)
    {
        return accumulate_parallel(v, make_box(v.front()), insertValue, insertBox);
    }
}
namespace accumulate_parallel_lambda
{    
    template<class Container, class Result, class PartFunc, class Func>
    Result accumulate_parallel(const Container& v, Result init, PartFunc partFunc, Func func)
    {
        auto parts = thread::hardware_concurrency();
        auto size  = v.size();
        auto first = begin(v);

        vector<future<Result>> futures;
        for (size_t i = 0; i != parts; ++i) {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [init, partFunc, first, part_size] 
                { 
                    return accumulate(first, next(first, part_size), init, partFunc); 
                }));
            advance(first, part_size);
        }

        return accumulate(begin(futures), end(futures), init, [func](Result& prevResult, future<Result>& fut)
            { 
                return func(prevResult, fut.get()); 
            });
    }

    Box<double> calcBox(const vector<double>& v)
    {
        auto insertValue = [](Box<double>& b, double d) { 
            b.min = d < b.min ? d : b.min;
            b.max = b.max < d ? d : b.max;
            return b;
        };
        auto insertBox = [](Box<double>& b, const Box<double>& c) {
            b.min = c.min < b.min ? c.min : b.min;
            b.max = b.max < c.max ? c.max : b.max;
            return b;
        };
        return accumulate_parallel(v, make_box(v.front()), insertValue, insertBox);
    }
}
namespace accumulate_parallel_lambda_one_less_thread
{    
    template<class Container, class Result, class PartFunc, class Func>
    Result accumulate_parallel(const Container& v, Result init, PartFunc partFunc, Func func)
    {
        auto parts = thread::hardware_concurrency() - 1;
        auto size  = v.size();
        auto first = begin(v);

        vector<future<Result>> futures;
        for (size_t i = 0; i != parts; ++i) {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [init, partFunc, first, part_size] 
                { 
                    return accumulate(first, next(first, part_size), init, partFunc); 
                }));
            advance(first, part_size);
        }

        return accumulate(begin(futures), end(futures), init, [func](Result& prevResult, future<Result>& fut)
            { 
                return func(prevResult, fut.get()); 
            });
    }

    Box<double> calcBox(const vector<double>& v)
    {
        auto insertValue = [](Box<double>& b, double d) { 
            b.min = d < b.min ? d : b.min;
            b.max = b.max < d ? d : b.max;
            return b;
        };
        auto insertBox = [](Box<double>& b, const Box<double>& c) {
            b.min = c.min < b.min ? c.min : b.min;
            b.max = b.max < c.max ? c.max : b.max;
            return b;
        };
        return accumulate_parallel(v, make_box(v.front()), insertValue, insertBox);
    }
}

namespace for_next_two_if_else
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        for (size_t i = 0; i < v.size(); i +=2) {
            if (v[i] < v[i+1]) 
            {
                if (v[i] < min) min = v[i];
                if (max < v[i+1]) max = v[i+1]; 
            }
            else  
            {
                if (v[i+1] < min) min = v[i+1];
                if (max < v[i]) max = v[i]; 
            }
        }
        return make_box(min, max);
    }
}
namespace for_next_two_assign_before_compare
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double a;
        double b;
        double mi;
        double ma;

        for (size_t i = 0; i < v.size();) {
            a = v[i++];
            b = v[i++];
            mi = a < b ? ma = b, a : ma = a, b;

            if (mi < min) min = mi;
            if (max < ma) max = ma; 
        }
        return make_box(min, max);
    }
}
namespace for_next_two_assign_before_compare2
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double a;
        double b;
        size_t i = 0;
        size_t j = v.size()/2;
        const size_t e = v.size()/2;
       
        while (i < e) {                       
            if ((a = v[i++]) < (b = v[j++])) //undefined behavior!!?!
            {
                if (a < min) min = a;
                if (max < b) max = b; 
            }
            else  
            {
                if (b < min) min = b;
                if (max < a) max = a; 
            }
        }
        return make_box(min, max);
    }
}
namespace for_next_two_assign_in_comparision
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double a;
        double b;

        for (size_t i = 0; i < v.size();) {
            if ((a = v[i++]) < (b = v[i++]))//undefined behavior!!?!
            {
                if (a < min) min = a;
                if (max < b) max = b; 
            }
            else  
            {
                if (b < min) min = b;
                if (max < a) max = a; 
            }
        }
        return make_box(min, max);
    }
}
namespace for_next_two_assign_minmax
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double mi;
        double ma;

        for (size_t i = 0; i < v.size(); i += 2) {
            tie(mi, ma) = minmax(v[i], v[i+1]);
            min = std::min(mi, min);
            max = std::max(ma, max);
        }
        return make_box(min, max);
    }
}
namespace while_next_two_assign_before_comparision
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double a = v[0];
        double b = v[1];
        size_t i = 0; 
        size_t end = v.size();
        while (i < end) {
            a = v[i++];
            b = v[i++];
            if (a < b) 
            {
                if (a < min) min = a;
                if (max < b) max = b; 
            }
            else  
            {
                if (b < min) min = b;
                if (max < a) max = a; 
            }
        }
        return make_box(min, max);
    }
}
namespace while_next_two_assign_in_comparision
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double a;
        double b;
        size_t i = 0; 
        size_t j = 1;
        size_t end = v.size();
        while (j < end) {
            if ((a = v[i]) < (b = v[j]))
            {
                if (a < min) min = a;
                if (max < b) max = b; 
            }
            else  
            {
                if (b < min) min = b;
                if (max < a) max = a; 
            }
            ++i; ++i;
            ++j; ++j;
        }
        return make_box(min, max);
    }
}
namespace for_next_two_assign_in_comparision2
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double a = v[0];
        double b = v[1];

        for (size_t i = 0; i < v.size();) {
            a = v[i++];
            if (a < (b=v[i++]))
            {
                if (a < min) min = a;
                if (max < b) max = b; 
            }
            else  
            {
                if (b < min) min = b;
                if (max < a) max = a; 
            }
        }
        return make_box(min, max);
    }
}
namespace for_next_two_conditional
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double a;
        double b;
        for (size_t i = 0; i < v.size(); ) {
            if ((a = v[i++]) < (b = v[i++]))//undefined behavior!!?!
            {
                min = a < min ? a : min;
                max = max < b ? b : max;
            }
            else
            {
                min = b < min ? b : min;
                max = max < a ? a : max;
            }
        }
        return make_box(min, max);
    }
}
namespace for_next_two_conditional2
{    
    Box<double> calcBox(const vector<double>& v)
    {
        double min = v.front();
        double max = v.front();
        double a;
        double b;
        bool c;
        for (size_t i = 0; i < v.size(); ) {
            c = (a = v[i++]) < (b = v[i++]);//undefined behavior!!?!
            c ? (a < min ? min = a : 0.0) , (max < b ? max = b : 0.0) : (b < min ? min = b : 0.0) , (max < a ? max = a : 0.0);
        }
        return make_box(min, max);
    }
}

namespace simd2_range_for
{    
    Box<double> calcBox(const vector<simd::double2>& v)
    {
        using namespace simd;

        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) {
            mi = min(d, mi);
            ma = max(d, ma); 
        }
        return make_double_box(mi, ma);
    }
}
namespace simd2_while
{    
    Box<double> calcBox(const vector<simd::double2>& v)
    {
        using namespace simd;

        auto mi = v.front();
        auto ma = v.front();
        auto it = begin(v), e = end(v);
        while (it != e) {
            const auto& d = *it;
            mi = min(d, mi);
            ma = max(d, ma); 
            ++it;
        }
        return make_double_box(mi, ma);
    }
}
namespace simd2_future_while
{    
    Box<double> calcBox(const vector<simd::double2>& v)
    {
        using namespace simd;

        auto parts = thread::hardware_concurrency();
        auto size = v.size();
        size_t first = 0;

        vector<future<Box<double>>> futures;
        for (size_t i = 0; i != parts; ++i) 
        {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [&v, first, part_size]
            { 
                auto mi = v[first];
                auto ma = mi;
                size_t i = first;
                size_t end = first + part_size;
                while (i != end) {
                    const auto& d = v[i];
                    mi = min(d, mi);
                    ma = max(d, ma);
                    ++i;
                }
                return make_double_box(mi, ma);
            }));
            first += part_size;
        }

        auto b = make_double_box(v.front(), v.front());
        auto& mi = b.min;
        auto& ma = b.max;
        for (auto& fut : futures) {
            auto b = fut.get();
            mi = min(b.min, mi);
            ma = max(b.max, ma);
        }
        return make_box(mi, ma);
    }
}
namespace simd2_accumulate_parallel
{    
    template<class Container, class Result, class PartFunc, class Func>
    Result accumulate_parallel(const Container& v, Result init, PartFunc partFunc, Func func)
    {
        auto parts = thread::hardware_concurrency();
        auto size  = v.size();
        auto first = begin(v);

        vector<future<Result>> futures;
        for (size_t i = 0; i != parts; ++i) {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [init, partFunc, first, part_size] 
                { 
                    return accumulate(first, next(first, part_size), init, partFunc); 
                }));
            advance(first, part_size);
        }

        return accumulate(begin(futures), end(futures), init, [func](Result& prevResult, future<Result>& fut)
            { 
                return func(prevResult, fut.get()); 
            });
    }

    Box<double> calcBox(const vector<simd::double2>& v)
    {
        using namespace simd;

        Box2d box2d(v.front());
        box2d = accumulate_parallel(v, box2d, insertValue, insertBox);
        return make_box(box2d.min(), box2d.max());
    }
}

namespace simd4_range_for
{    
    Box<double> calcBox(const vector<simd::double4>& v)
    {
        using namespace simd;

        auto mi = v.front();
        auto ma = v.front();
        for (const auto& d : v) {
            mi = min(d, mi);
            ma = max(d, ma); 
        }
        return make_double_box(mi, ma);
    }
}
namespace simd4_range_for_range_for
{    
    Box<double> calcBox(const vector<simd::double4>& v)
    {
        using namespace simd;

        auto mi = v.front();
        for (const auto& d : v) 
            mi = min(d, mi);

        auto ma = v.front();
        for (const auto& d : v) 
            ma = max(d, ma); 

        return make_double_box(mi, ma);
    }
}
namespace simd4_while
{    
    Box<double> calcBox(const vector<simd::double4>& v)
    {
        using namespace simd;

        auto mi = v.front();
        auto ma = v.front();
        auto it = begin(v), e = end(v);
        while (it != e) {
            const auto& d = *it;
            mi = min(d, mi);
            ma = max(d, ma); 
            ++it;
        }
        return make_double_box(mi, ma);
    }
}
namespace simd4_while_while
{    
    Box<double> calcBox(const vector<simd::double4>& v)
    {
        using namespace simd;

        auto mi = v.front();
        {
            auto it = begin(v), e = end(v);
            while (it != e) {
                mi = min(*it, mi);
                ++it;
            }
        }
        auto ma = v.front();
        {
            auto it = begin(v), e = end(v);
            while (it != e) {
                ma = max(*it, ma);
                ++it;
            }
        }
        return make_double_box(mi, ma);
    }
}
namespace simd4_future_while
{    
    Box<double> calcBox(const vector<simd::double4>& v)
    {
        using namespace simd;

        auto parts = thread::hardware_concurrency();
        auto size = v.size();
        size_t first = 0;

        vector<future<Box<double>>> futures;
        for (size_t i = 0; i != parts; ++i) 
        {
            const auto part_size = (size * i + size) / parts - (size * i) / parts;
            futures.emplace_back(async(launch::async, [&v, first, part_size]
            { 
                auto mi = v[first];
                auto ma = mi;
                size_t i = first;
                size_t end = first + part_size;
                while (i != end) {
                    const double4& d = v[i];
                    mi = min(d, mi);
                    ma = max(d, ma);
                    ++i;
                }
                return make_double_box(mi, ma);
            }));
            first += part_size;
        }

        auto b = make_double_box(v.front(), v.front());
        auto& mi = b.min;
        auto& ma = b.max;
        for (auto& fut : futures) {
            auto b = fut.get();
            mi = min(b.min, mi);
            ma = max(b.max, ma);
        }
        return make_box(mi, ma);
    }
}


