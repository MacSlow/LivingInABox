#include "calcBox.h"
#include "measure.h"
#include "test_data.h"
#include "test_calcBox.h"
#include "xml.h"

using namespace simd;
using namespace std;
using namespace xml;

//please adopt to your system setup
const std::string sProcessor       = "Intel Core i7-2670 QM CPU @ 2.20 GHz";
const std::string sCompiler        = "MVC 2012";
const std::string sCompilerOptions = "/O2 /Oi /Ot /arch:AVX"; 


namespace { //anonymous namespace for local helper functions

    //helper for vector<double>
    template<class Algorithm>
    XmlLine testLine(const vector<double>& v, const Box<double>& controlBox, Algorithm algorithm, const std::string& name)
    {
        auto box_ms = measure_milliseconds(v, algorithm);
        auto box    = box_ms.first;
        auto ms     = box_ms.second;
        auto line = XmlLine(); //please do not use blank or special character as key     
        line.add("Processor"      , sProcessor);
        line.add("Compiler"       , sCompiler);
        line.add("CompilerOptions", sCompilerOptions);
        line.add("Data"           , "vector_double");
        line.add("Doubles"        , v.size());
        line.add("Size"           , v.size());
        line.add("min"            , box.min);
        line.add("max"            , box.max);
        line.add("correct"        , box.min == controlBox.min && box.max == controlBox.max);
        line.add("Milliseconds"   , ms);
        line.add("Algorithm"      , name);
        line.add("DoublesPerNano" , 0.001 * v.size() / (double)ms); 
        return line;
    }    
    
    //helper for vector<simd::double2>
    template<class Algorithm>
    XmlLine testLine(const vector<double2>& v, const Box<double>& controlBox, Algorithm algorithm, const std::string& name)
    {
        auto box_ms = measure_milliseconds(v, algorithm);
        auto box    = box_ms.first;
        auto ms     = box_ms.second;
        auto line   = XmlLine();     //please do not use blank or special character as key     
        line.add("Processor"      , sProcessor);
        line.add("Compiler"       , sCompiler);
        line.add("CompilerOptions", sCompilerOptions);
        line.add("Data"           , "vector_double");
        line.add("Doubles"        , 2 * v.size());
        line.add("Size"           , v.size());
        line.add("min"            , box.min);
        line.add("max"            , box.max);
        line.add("correct"        , box.min == controlBox.min && box.max == controlBox.max);
        line.add("Milliseconds"   , ms);
        line.add("Algorithm"      , name);
        line.add("DoublesPerNano" , 0.002 * v.size() / (double)ms); 
        return line;
    }    
    
    //helper for vector<simd::double4>
    template<class Algorithm>
    XmlLine testLine(const vector<double4>& v, const Box<double>& controlBox, Algorithm algorithm, const std::string& name)
    {
        auto box_ms = measure_milliseconds(v, algorithm);
        auto box    = box_ms.first;
        auto ms     = box_ms.second;
        auto line   = XmlLine();     //please do not use blank or special character as key     
        line.add("Processor"      , sProcessor);
        line.add("Compiler"       , sCompiler);
        line.add("CompilerOptions", sCompilerOptions);
        line.add("Data"           , "vector_double");
        line.add("Doubles"        , 4 * v.size());
        line.add("Size"           , v.size());
        line.add("min"            , box.min);
        line.add("max"            , box.max);
        line.add("correct"        , box.min == controlBox.min && box.max == controlBox.max);
        line.add("Milliseconds"   , ms);
        line.add("Algorithm"      , name);
        line.add("DoublesPerNano" , 0.004 * v.size() / (double)ms); 
        return line;
    }

}//end of anonymous namespace for local helper functions


//macro to easy formulate the testcases
#define TEST(DATA, NAMESPACE) { using namespace NAMESPACE; xml.add(testLine(DATA, box, calcBox, #NAMESPACE)); }


void test_calcBox()
{
    auto maxTest = 2;
    auto xml = XmlMain();

    for (auto i = 0; i < maxTest; ++i) 
    {
        auto v = testVectorDouble(i); 
        auto box = range_for_if_if::calcBox(v); //to validate result

        TEST(v, for_for)
        TEST(v, for_min_max)
        TEST(v, for_own_min_max)
        TEST(v, for_lower_if_if)
        TEST(v, for_lower_if_else)
        TEST(v, for_if_if)
        TEST(v, for_if_else)
        TEST(v, for_if_continue)
        TEST(v, for_if_continue_nan_check)
        TEST(v, for_conditional)
        TEST(v, for_fixend_if_if)
        TEST(v, for_fixend_if_else)
        TEST(v, for_fixend_if_continue)
        TEST(v, for_fixend_conditional)
        TEST(v, for_if_if_pragma_no_vector)
        TEST(v, for_if_else_pragma_no_vector)
        TEST(v, for_if_continue_pragma_no_vector)
        TEST(v, for_conditional_pragma_no_vector)
        TEST(v, for_if_if_pragma_hint_parallel)

        TEST(v, range_for_if_if)
        TEST(v, range_for_min_max)
        TEST(v, range_for_if_else)
        TEST(v, range_for_if_continue)
        TEST(v, range_for_conditional)
        TEST(v, range_for_conditional_conditional)
        TEST(v, range_for_conditional_conditional_reverse)
        TEST(v, range_for_operator)

        TEST(v, while_if_if)
        TEST(v, while_if_else)
        TEST(v, while_if_continue)
        TEST(v, while_conditional)

        TEST(v, std_min_element_max_element)
        TEST(v, std_minmax_element)

        TEST(v, std_accumulate_function)
        TEST(v, std_accumulate_lambda)
        TEST(v, std_accumulate_lambda_return)
        TEST(v, std_accumulate_lambda_copy)

        TEST(v, std_future_accumulate_insert)
        TEST(v, std_future_accumulate_lambda)
        TEST(v, std_future_while)
        TEST(v, accumulate_parallel)
        TEST(v, accumulate_parallel_lambda)
        TEST(v, accumulate_parallel_lambda_one_less_thread)

        TEST(v, for_next_two_if_else)
        TEST(v, for_next_two_assign_before_compare)
        TEST(v, for_next_two_assign_before_compare2) //undefined behavior!!?!
        TEST(v, for_next_two_assign_minmax)          //undefined behavior!!?!
        TEST(v, for_next_two_assign_in_comparision)
        TEST(v, for_next_two_assign_in_comparision2)
        TEST(v, for_next_two_conditional)            //undefined behavior!!?!
        TEST(v, for_next_two_conditional2)           //undefined behavior!!?!
        TEST(v, while_next_two_assign_before_comparision)
        TEST(v, while_next_two_assign_in_comparision)
    }
       
    for (auto i = 0; i < maxTest; ++i) 
    {
        auto v = testVectorDouble2(i); 
        auto box = simd2_range_for::calcBox(v); //to validate result
        TEST(v, simd2_range_for)
        TEST(v, simd2_while)
        TEST(v, simd2_future_while)
        TEST(v, simd2_accumulate_parallel)
    }
           
    for (auto i = 0; i < maxTest; ++i) 
    {
        auto v = testVectorDouble4(i); 
        auto box = simd4_range_for::calcBox(v); //to validate result
        TEST(v, simd4_range_for)
        TEST(v, simd4_range_for_range_for)
        TEST(v, simd4_while)
        TEST(v, simd4_while_while)
        TEST(v, simd4_future_while)
    }

    cout << xml.text() << endl;
}

