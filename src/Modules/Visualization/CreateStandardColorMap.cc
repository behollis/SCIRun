/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
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

/// @todo Documentation Modules/Visualization/CreateStandardColorMap.cc

#include <Modules/Visualization/CreateStandardColorMap.h>
#include <Core/Datatypes/ColorMap.h>

using namespace SCIRun::Modules::Visualization;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Core::Algorithms::Visualization;

const ModuleLookupInfo CreateStandardColorMap::staticInfo_("CreateStandardColorMap", "Visualization", "SCIRun");

CreateStandardColorMap::CreateStandardColorMap() : Module(staticInfo_)
{
  INITIALIZE_PORT(ColorMapObject);
}

void CreateStandardColorMap::setStateDefaults()
{
  auto state = get_state();
  state->setValue(Parameters::ColorMapName, std::string("Rainbow"));
  state->setValue(Parameters::ColorMapResolution, 256);
  state->setValue(Parameters::ColorMapInvert, false);
  state->setValue(Parameters::ColorMapShift, 0.0);
  state->setValue(Parameters::AlphaUserPointsVector, Variable::List());
}

void CreateStandardColorMap::execute()
{
  if (needToExecute())
  {
    auto state = get_state();
    auto name = state->getValue(Parameters::ColorMapName).toString();
    auto res = state->getValue(Parameters::ColorMapResolution).toInt();
    auto inv = state->getValue(Parameters::ColorMapInvert).toBool();
    auto shift = state->getValue(Parameters::ColorMapShift).toDouble();

    //TODO cbright: pass computed alpha function from transient state to factory
    auto alphaPoints = state->getValue(Parameters::AlphaUserPointsVector).toVector();

    //just in case there is a problem with the QT values...
    res = std::min(std::max(res,2),256);
    shift = std::min(std::max(shift,-1.),1.);
    sendOutput(ColorMapObject, StandardColorMapFactory::create(name, res, shift, inv));
  }
}

ALGORITHM_PARAMETER_DEF(Visualization, ColorMapName);
ALGORITHM_PARAMETER_DEF(Visualization, ColorMapInvert);
ALGORITHM_PARAMETER_DEF(Visualization, ColorMapShift);
ALGORITHM_PARAMETER_DEF(Visualization, ColorMapResolution);
ALGORITHM_PARAMETER_DEF(Visualization, AlphaUserPointsVector);
ALGORITHM_PARAMETER_DEF(Visualization, AlphaFunctionVector);
