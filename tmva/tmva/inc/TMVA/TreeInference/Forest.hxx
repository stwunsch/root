#ifndef TMVA_TREEINFERENCE_FOREST
#define TMVA_TREEINFERENCE_FOREST

#include <functional>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>

#include "TFile.h"
#include "TDirectory.h"

#include "BranchlessTree.hxx"
#include "Objectives.hxx"

namespace TMVA {
namespace Experimental {

/// Forest base class
///
/// \tparam T Value type for the computation (usually floating point type)
/// \tparam ForestType Type of the collection of trees
template <typename T, typename ForestType>
struct ForestBase {
   using Value_t = T;
   // TODO: Do we need the objective name?
   std::function<T(T)> fObjectiveFunc; ///< Objective function
   ForestType fTrees;                  ///< Store the forest, either as vector or jitted function
   int fNumFeatures;                   ///< Number of input variables / input features

   void Inference(const T *inputs, const int rows, T *predictions);
};

/// Perform inference of the forest on a batch of inputs
///
/// \param[in] inputs Pointer to data containing the inputs
/// \param[in] rows Number of events in inputs vector
/// \param[in] predictions Pointer to the buffer to be filled with the predictions
template <typename T, typename ForestType>
inline void ForestBase<T, ForestType>::Inference(const T *inputs, const int rows, T *predictions)
{
   for (int i = 0; i < rows; i++) {
      predictions[i] = 0.0;
      for (auto &tree : fTrees) {
         predictions[i] += tree.Inference(inputs + i * fNumFeatures);
      }
      predictions[i] = fObjectiveFunc(predictions[i]);
   }
}

/// Forest using branchless trees
///
/// \tparam T Value type for the computation (usually floating point type)
template <typename T>
struct BranchlessForest : public ForestBase<T, std::vector<BranchlessTree<T>>> {
   void Load(const std::string &key, const std::string &filename, bool sortTrees = true);
};

/// Load parameters from a ROOT file to the branchless trees
///
/// \param[in] key Name of folder in the ROOT file containing the model parameters
/// \param[in] filename Filename of the ROOT file
/// \param[in] sortTrees Flag to indicate sorting the input trees by the cut value of the first node of each tree
template <typename T>
inline void BranchlessForest<T>::Load(const std::string &key, const std::string &filename, bool sortTrees)
{
   // Open input file and get folder from key
   auto file = TFile::Open(filename.c_str(), "READ");
   auto dir = file->GetDirectory(key.c_str());
   if (dir == 0)
      throw std::runtime_error("Failed to open directory with name " + key + ".");

   // Load parameters from file
   auto maxDepth = (std::vector<int> *)file->Get((key + "/max_depth").c_str());
   auto numTrees = (std::vector<int> *)file->Get((key + "/num_trees").c_str());
   auto numFeatures = (std::vector<int> *)file->Get((key + "/num_features").c_str());
   auto objective = (std::string *)file->Get((key + "/objective").c_str());
   auto features = (std::vector<int> *)file->Get((key + "/features").c_str());
   auto thresholds = (std::vector<float> *)file->Get((key + "/thresholds").c_str());

   this->fNumFeatures = numFeatures->at(0);
   this->fObjectiveFunc = Objectives::GetFunction<T>(*objective);

   // Load parameters in trees
   this->fTrees.resize(numTrees->at(0));
   const auto lenFeatures = std::pow(2, maxDepth->at(0)) - 1;
   const auto lenThresholds = std::pow(2, maxDepth->at(0) + 1) - 1;
   for (int i = 0; i < numTrees->at(0); i++) {
      // Set tree depth
      this->fTrees[i].fTreeDepth = maxDepth->at(0);
      // Set feature indices
      this->fTrees[i].fFeatures.resize(lenFeatures);
      std::copy(features->begin() + i * lenFeatures, features->end() + i * lenFeatures,
                this->fTrees[i].fFeatures.begin());
      // Set threshold values
      this->fTrees[i].fThresholds.resize(lenThresholds);
      std::copy(thresholds->begin() + i * lenThresholds, thresholds->end() + i * lenThresholds,
                this->fTrees[i].fThresholds.begin());
      // Fill sparse trees fully
      this->fTrees[i].FillSparse();
   }

   // Clean-up
   file->Close();
   delete maxDepth;
   delete numTrees;
   delete numFeatures;
   delete objective;
   delete features;
   delete thresholds;
}

} // namespace Experimental
} // namespace TMVA

#endif // TMVA_TREEINFERENCE_FOREST
