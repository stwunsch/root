using namespace ROOT::VecOps;

void vo005_Combinations(){
    RVec<float> x = {1, 2, 3, 4};
    cout << "Initial vector:\n  " << x << endl << endl;

    auto c1 = Combinations(x, 3, 0);
    auto c2 = Combinations(x, 3, 1);
    auto c3 = Combinations(x, 3, 2);
    auto z = c1 * c2 * c3;
    cout << "Combinations:" << endl;
    for(size_t i=0; i<z.size(); i++) {
        cout << "  " << c1[i] << " * " << c2[i] << " * " << c3[i] << " = " << z[i] << endl;
    }
}
