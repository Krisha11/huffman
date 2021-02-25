#ifndef TEST_HPP
#define TEST_HPP

#include "cstdio"

class Test {
protected:
    static int failedNum;
    static int totalNum;
    static bool is_ok_now;                        
                          
    static void check(bool expr, const char  *filename, std::size_t lineNum); 
    static void start_test();
    static void end_test();
public:
    static void showFinalResult();
    
    virtual void runAllTests() = 0;
};


#endif
