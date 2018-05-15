#ifndef ROOT_PyROOT_Test
#define ROOT_PyROOT_Test

#include <functional>

float TestFunction(float x){
    return -1.0*x;
};

class Test {
public:
    Test(int n){
        numCalls = n;
        function = TestFunction;
    }

    void SetFunction(float (*f)(float)){
        function = f;
    }

    float Apply(float x){
        for(int i=0; i<numCalls-1; i++) (*function)(x);
        return (*function)(x);
    }

private:
    int numCalls;
    float (*function)(float);
};

#endif
