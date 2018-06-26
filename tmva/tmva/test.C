#include <TMVA/RTensor.h>

using namespace TMVA::Experimental;

void test()
{
   // Initialize from data
   std::vector<float> data({1.f, 2.f, 3.f, 4.f, 5.f, 6.f});
   auto x = RTensor<float>(data.data(), {2u, 3u});

   std::cout << "Initialize tensor:" << std::endl << std::endl;
   std::cout << x << std::endl << std::endl;

   // Access elements
   std::cout << "Access elements:" << std::endl << std::endl;

   std::cout << "{ { " << x.At(0, 0) << ", " << x.At(0, 1) << ", " << x.At(0, 2) << " }" << std::endl;
   std::cout << "  { " << x.At(1, 0) << ", " << x.At(1, 1) << ", " << x.At(1, 2) << " } }" << std::endl << std::endl;

   std::cout << "{ { " << x(0, 0) << ", " << x(0, 1) << ", " << x(0, 2) << " }" << std::endl;
   std::cout << "  { " << x(1, 0) << ", " << x(1, 1) << ", " << x(1, 2) << " } }" << std::endl << std::endl;

   // Reshape
   x.Reshape({3u, 2u});

   std::cout << "Reshape tensor:" << std::endl << std::endl;
   std::cout << x << std::endl << std::endl;

   // Initialize flat tensor
   std::vector<float> data2({1.f, 2.f});
   auto y = RTensor<float>(data2.data(), {2u});

   std::cout << "Initialize flat tensor:" << std::endl << std::endl;
   std::cout << y << std::endl << std::endl;

   // Expand dimensions
   y.ExpandDims(-1);

   std::cout << "Expand dimensions:" << std::endl << std::endl;
   std::cout << y << std::endl << std::endl;

   // Squeeze dimensions
   y.Squeeze();

   std::cout << "Squeeze dimensions:" << std::endl << std::endl;
   std::cout << y << std::endl << std::endl;
}

int main()
{
   test();
}
