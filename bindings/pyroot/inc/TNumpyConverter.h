#pragma once

#include <vector>
#include "ROOT/TVec.hxx"

using ROOT::Experimental::VecOps::TVec;

template<typename dtype>
class TNumpyConverter {
public:
    void SetData(dtype* x){data = x;}
    void SetDims(std::vector<unsigned int> x){dims = x;}

    TVec<dtype> AsTVec(){
        unsigned int size = dims[0];
        return TVec<dtype>(data, size);
    }

private:
    dtype* data;
    std::vector<unsigned int> dims;
};
