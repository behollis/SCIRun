#include <Core/Algorithms/Math/SortMatrixAlgo.h>
#include <Core/Datatypes/MatrixTypeConversions.h>
#include <Core/Math/MiscMath.h>

using namespace SCIRun;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Core::Algorithms::Math;

SortMatrixAlgo::SortMatrixAlgo()
{
  //set parameter defaults for UI
  addParameter(Variables::Method, 0);
}

AlgorithmOutput
SortMatrixAlgo::run(const AlgorithmInput& input) const
{
  auto input_matrix = input.get<Matrix>(Variables::InputMatrix);
  AlgorithmOutput output;

  //sparse support not fully implemented yet.
  if (!matrixIs::dense(input_matrix))
  {
    //TODO implement something with sparse
    error("SortMatrix: Currently only works with dense matrices");
    output[Variables::OutputMatrix] = 0;
    return output;
  }

  auto mat = castMatrix::toDense(input_matrix);
  DenseMatrixHandle return_matrix;

  //pull parameter from UI
  auto method = get(Variables::Method).toInt();
  Sort(mat,return_matrix,method);
  output[Variables::OutputMatrix] = return_matrix;
  return output;
}

bool
SortMatrixAlgo::Sort(DenseMatrixHandle input, DenseMatrixHandle& output, int method) const
{
  if (!input)
  {
    error("SortAscending: no input matrix found");
    return false;
  }

  //get size of original matrix
  size_type nrows = input->nrows();
  size_type ncols = input->ncols();

  //copy original matrix for processing
  output.reset(new DenseMatrix(*input));

  //pointer to matrix data
  double *data = output->data();
  if (!output)
  {
    error("ApplyRowOperation: could not create output matrix");
    return false;
  }

  size_type n = nrows*ncols;

  //call the sorting functions
  Quicksort(data,0,n-1);

  if (method==1)
  {
    //if set to descending, reverse the order.
    output.reset(new DenseMatrix(output -> reverse()));
  }

  return true;
}

bool
SortMatrixAlgo::Quicksort(double* input, index_type lo, index_type hi) const
{
  //splits matrix based on Partition function
  index_type ind;
  if (lo<hi)
  {
    ind = Partition(input,lo,hi);
    Quicksort(input,lo,ind-1);
    Quicksort(input,ind+1,hi);
  }

  return true;
}

index_type
SortMatrixAlgo::Partition(double* input, index_type lo, index_type hi) const
{
  // places the last entry in its proper place in relation to the other
  // entries, ie, smaller values before and larger values after.
  index_type ind=lo;
  double pivot = input[hi];
  double tmp;

  for (index_type k=lo;k<hi;k++)
  {
    if (input[k]<=pivot)
    {
      tmp=input[ind];
      input[ind]=input[k];
      input[k]=tmp;
      ind+=1;
    }
  }

  tmp=input[ind];
  input[ind]=input[hi];
  input[hi]=tmp;

  return ind;
}
