#ifndef ROOT_TMVA_FISHER
#define ROOT_TMVA_FISHER

#include "TMVA/RTensor.h"
#include "TMatrix.h"
#include <iostream>

namespace TMVA {
namespace Experimental {

namespace Internal {

template <typename T>
TMVA::Experimental::RTensor<T> Invert(TMVA::Experimental::RTensor<T> &x)
{
   const auto shape = x.GetShape();
   TMVA::Experimental::RTensor<T> r(shape);
   TMatrixT<T> y(shape[0], shape[1]);
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         y[i][j] = x(i, j);
      }
   }
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         r(i, j) = y[i][j];
      }
   }
   return r;
}

template <typename T>
TMVA::Experimental::RTensor<T> Add(TMVA::Experimental::RTensor<T> &x, TMVA::Experimental::RTensor<T> &y)
{
   auto const shape = x.GetShape();
   TMVA::Experimental::RTensor<T> r(shape);
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         r(i, j) = x(i, j) + y(i, j);
      }
   }
   return r;
}

template <typename T>
TMVA::Experimental::RTensor<T>
ComputeCovarianceMatrix(TMVA::Experimental::RTensor<T> &x, TMVA::Experimental::RTensor<T> &y, float classLabel)
{
   const auto shape = x.GetShape();

   // Compute means
   std::vector<T> means(shape[1]);
   size_t count = 0;
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         if (y(i) == classLabel) {
            means[j] += x(i, j);
            count++;
         }
      }
   }
   for (size_t j = 0; j < shape[1]; j++) {
      means[j] = means[j] / float(count);
   }

   // Compute covariance matrix
   TMVA::Experimental::RTensor<T> cov({shape[1], shape[1]});
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         for (size_t k = 0; k < shape[1]; k++) {
            if (y(i) == classLabel) {
               cov(j, k) += (x(i, j) - means[j]) * (x(i, k) - means[k]);
            }
         }
      }
   }
   for (size_t j = 0; j < shape[1]; j++) {
      for (size_t k = 0; k < shape[1]; k++) {
         cov(j, k) = cov(j, k) / float(count);
      }
   }
   return cov;
}

template <typename T>
std::vector<T> ComputeMean(TMVA::Experimental::RTensor<T> &x, TMVA::Experimental::RTensor<T> &y, float classLabel)
{
   const auto shape = x.GetShape();
   std::vector<T> means(shape[1]);
   size_t count = 0;
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         if (y(i) == classLabel) {
            means[j] += x(i, j);
            count++;
         }
      }
   }
   for (size_t j = 0; j < shape[1]; j++) {
      means[j] = means[j] / float(count);
   }
   return means;
}

template <typename T>
std::vector<T> Subtract(std::vector<T> &x, std::vector<T> &y)
{
   std::vector<T> r(x.size());
   for (size_t i = 0; i < x.size(); i++) {
      r[i] = x[i] - y[i];
   }
   return r;
}

template <typename T>
std::vector<T> Matmul(TMVA::Experimental::RTensor<T> &x, std::vector<T> &y)
{
   const auto size = y.size();
   std::vector<T> r(size);
   for (size_t i = 0; i < size; i++) {
      for (size_t j = 0; j < size; j++) {
         r[i] = x(i, j) * y[j];
      }
   }
   return r;
}

} // namespace Internal

namespace Training {

template <typename T>
class Fisher {
public:
   Fisher();
   void Fit(TMVA::Experimental::RTensor<T> &inputs, TMVA::Experimental::RTensor<T> &labels);

   std::vector<T> GetFisherCoeff() { return fFisherCoeff; };

private:
   std::vector<T> fFisherCoeff;
};

template <typename T>
Fisher<T>::Fisher(){};

template <typename T>
void Fisher<T>::Fit(TMVA::Experimental::RTensor<T> &inputs, TMVA::Experimental::RTensor<T> &labels)
{
   auto covSig = Internal::ComputeCovarianceMatrix<T>(inputs, labels, 1);
   auto covBkg = Internal::ComputeCovarianceMatrix<T>(inputs, labels, 0);
   auto cov = Internal::Add(covSig, covBkg);
   auto covInv = Internal::Invert(cov);

   auto meanSig = Internal::ComputeMean(inputs, labels, 1);
   auto meanBkg = Internal::ComputeMean(inputs, labels, 0);
   auto meanDiff = Internal::Subtract(meanSig, meanBkg);

   fFisherCoeff = Internal::Matmul(covInv, meanDiff);
};

} // namespace Training

namespace Inference {

template <typename T>
class Fisher {
public:
   Fisher(TMVA::Experimental::Training::Fisher<T> &model);
   TMVA::Experimental::RTensor<T> Predict(TMVA::Experimental::RTensor<T> &x);

private:
   std::vector<T> fFisherCoeff;
};

template <typename T>
Fisher<T>::Fisher(TMVA::Experimental::Training::Fisher<T> &model)
{
   fFisherCoeff = model.GetFisherCoeff();
};

template <typename T>
TMVA::Experimental::RTensor<T> Fisher<T>::Predict(TMVA::Experimental::RTensor<T> &x)
{
   const auto shape = x.GetShape();
   TMVA::Experimental::RTensor<T> y({shape[0]});
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         y(i) += x(i, j) * fFisherCoeff[j];
      }
   }
   return y;
}

} // namespace Inference

} // namespace Experimental
} // namespace TMVA

#endif
