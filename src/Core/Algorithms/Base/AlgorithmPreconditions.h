/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#ifndef ALGORITHMS_BASE_ALGORITHMPRECONDITIONS_H
#define ALGORITHMS_BASE_ALGORITHMPRECONDITIONS_H

#include <string>
#include <stdexcept>
#include <Core/Utils/Exception.h>
#include <Core/Algorithms/Base/Share.h>

namespace SCIRun {
namespace Core {
namespace Algorithms {

  struct SCISHARE AlgorithmInputException : virtual ExceptionBase
  {
  };

  struct SCISHARE AlgorithmProcessingException : virtual ExceptionBase
  {
  };

  struct SCISHARE AlgorithmParameterNotFound : virtual AlgorithmProcessingException //TODO??
  {
  };

#define ALGORITHM_INPUT_ERROR_WITH_TYPE(type, message) { error(message); BOOST_THROW_EXCEPTION(AlgorithmInputException() << type((message))); }
#define ALGORITHM_INPUT_ERROR(message) ALGORITHM_INPUT_ERROR_WITH_TYPE(SCIRun::Core::ErrorMessage, message)

#define ENSURE_NOT_NULL(var, message)  if (!(var)) ALGORITHM_INPUT_ERROR_WITH_TYPE(NullObjectInfo, message)
  
#define ENSURE_POSITIVE_DOUBLE(var, message)  if ((var) < 0) \
  BOOST_THROW_EXCEPTION(AlgorithmInputException() << DoubleOutOfRangeInfo( \
    DoubleOutOfRangeInfo::value_type( \
    std::string(message), \
    var, \
    boost::numeric::interval<double>(0, std::numeric_limits<double>::infinity())))) 

#define ENSURE_POSITIVE_INT(var, message) if ((var) < 0) \
    BOOST_THROW_EXCEPTION(AlgorithmInputException() << IntOutOfRangeInfo( \
    IntOutOfRangeInfo::value_type( \
    std::string(message), \
    var, \
    boost::numeric::interval<int>(0, std::numeric_limits<int>::infinity()))))

#define ENSURE_FILE_EXISTS(filename) if (!boost::filesystem::exists(filename))\
  ALGORITHM_INPUT_ERROR_WITH_TYPE(FileNotFound, std::string("File not found: (") + (filename.size() != 0 ? filename : "<no file set>") + ")")

}}}

#endif