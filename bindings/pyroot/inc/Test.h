#ifndef ROOT_PyROOT_Test
#define ROOT_PyROOT_Test

#include <functional>

float TestFunction(float x){
    return -1.0*x;
};

class Test {
public:
    Test(){
        function = TestFunction;
    }

    void SetFunction(float (*f)(float)){
        function = f;
    }

    float Apply(float x){
        return (*function)(x);
    }

private:
    float (*function)(float);
};

#endif
