using namespace ROOT::VecOps;

void vo005_Combinations()
{
   RVec<float> x = {1, 2, 3, 4};
   RVec<float> y = {-1, -2, -3, -4};
   cout << "Initial collections:\n  " << x << endl << y << endl;

   auto c = Combinations(x, y);
   auto z = Take(x, c[0]) * Take(y, c[1]);

   cout << "Combinations:" << endl;
   for (size_t i = 0; i < z.size(); i++) {
      cout << "  " << x[c[0][i]] << " * " << y[c[1][i]] << " = " << z[i] << endl;
   }
}
