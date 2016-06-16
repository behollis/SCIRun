#ifndef CORE_ALGORITHMS_MATH_SortMatrixALGO_H
#define CORE_ALGORITHMS_MATH_SortMatrixALGO_H
#include <Core/Datatypes/Matrix.h>
#include <Core/Datatypes/DenseMatrix.h>
#include <Core/Datatypes/DenseColumnMatrix.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <Core/Algorithms/Base/AlgorithmVariableNames.h>
#include <Core/Algorithms/Base/AlgorithmBase.h>
#include <Core/Algorithms/Math/share.h>
namespace SCIRun {
namespace Core {
namespace Algorithms {
namespace Math {
  class SCISHARE SortMatrixAlgo : public AlgorithmBase
  {
    public:
    SortMatrixAlgo();
    AlgorithmOutput run_generic(const AlgorithmInput& input) const;
    bool Sort(Datatypes::DenseMatrixHandle input,
    Datatypes::DenseMatrixHandle& output, int method) const;
    bool Quicksort(double* input, index_type lo, index_type hi) const;
    index_type Partition(double* input, index_type lo, index_type hi) const;
    };
  }
}}}
#endif
