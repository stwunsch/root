using namespace TMVA::Experimental;

void rtensor()
{
   // Create tensor with adopted memory
   float data[6] = {0, 1, 2, 3, 4, 5};
   RTensor<float> x(data, {3, 2});
   cout << "RTensor:\n" << x << "\n\n";

   // Get and set elements
   cout << "Get first element: " << x(0, 0) << "\n\n";

   x(0, 0) = 42.0;
   cout << "Set first element: " << x(0, 0) << "\n\n";

   cout << "RTensor:\n" << x << "\n\n";
}
