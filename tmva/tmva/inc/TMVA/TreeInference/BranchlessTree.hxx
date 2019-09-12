#ifndef TMVA_TREEINFERENCE_BRANCHLESSTREE
#define TMVA_TREEINFERENCE_BRANCHLESSTREE

#include <vector>
#include <algorithm>

namespace TMVA {
namespace Experimental {

namespace Internal {

/// Fill the empty nodes of a sparse tree recursively
template <typename T>
void RecursiveFill(int thisIndex, int lastIndex, int treeDepth, int maxTreeDepth, std::vector<T> &thresholds,
                   std::vector<int> &features)
{
   if (features[lastIndex] == -1) {
      features[thisIndex] = -1;
      thresholds[thisIndex] = thresholds[lastIndex];
   }
   if (treeDepth < maxTreeDepth) {
      Internal::RecursiveFill<T>(2 * thisIndex + 1, thisIndex, treeDepth + 1, maxTreeDepth, thresholds, features);
      Internal::RecursiveFill<T>(2 * thisIndex + 2, thisIndex, treeDepth + 1, maxTreeDepth, thresholds, features);
   }
}

} // namespace Internal

/// \class Tree
/// \brief Branchless representation of a decision tree using topological ordering
///
/// \tparam T Value type for the computation (usually floating point type)
template <typename T>
struct BranchlessTree {
   int fTreeDepth;             ///< Depth of the tree
   std::vector<T> fThresholds; ///< Cut thresholds or scores if corresponding node is a leaf
   std::vector<int> fFeatures; ///< Cut variables / features

   inline T Inference(const T *input);
   inline void FillSparse();
};

/// Perform inference on a single input vector
/// \param[in] input Pointer to data containing the input values
/// \param[out] Tree score, result of the inference
template <typename T>
inline T BranchlessTree<T>::Inference(const T *input)
{
   int index = 0;
   for (int level = 0; level < fTreeDepth; ++level) {
      index = 2 * index + 1 + (input[fFeatures[index]] > fThresholds[index]);
   }
   return fThresholds[index];
}

/// Fill nodes of a sparse tree forming a full tree
///
/// Sparse parts of the tree are marked with -1 values in the feature vector. The
/// algorithm fills these parts up with the last threshold value so that the result
/// of the inference stays the same but the computation always traverses the full tree,
/// which is needed to avoid branching logic.
template <typename T>
inline void BranchlessTree<T>::FillSparse()
{
   // Fill threshold / leaf values recursively
   Internal::RecursiveFill<T>(1, 0, 1, fTreeDepth, fThresholds, fFeatures);
   Internal::RecursiveFill<T>(2, 0, 1, fTreeDepth, fThresholds, fFeatures);

   // Replace feature indices of -1 with 0
   std::replace(fFeatures.begin(), fFeatures.end(), -1.0, 0.0);
}

} // namespace Experimental
} // namespace TMVA

#endif // TMVA_TREEINFERENCE_BRANCHLESSTREE
