#ifndef MODULES_STRING_TestModuleSimpleUI_H
#define MODULES_STRING_TestModuleSimpleUI_H
#include <Dataflow/Network/Module.h>
#include <Modules/Fields/share.h>
//#include <Core/Algorithms/Base/Name.h>

namespace SCIRun {
namespace Modules {
namespace StringManip {
class SCISHARE TestModuleSimpleUI : public SCIRun::Dataflow::Networks::Module,
//public Has1InputPort<StringPortTag>
public Has1InputPort<StringPortTag>,
public Has1OutputPort<StringPortTag>
{
public:
  TestModuleSimpleUI();
  virtual void execute();
  virtual void setStateDefaults();

  INPUT_PORT(0, InputString, String);
  OUTPUT_PORT(0, OutputString, String);

  static const Dataflow::Networks::ModuleLookupInfo staticInfo_;

  static Core::Algorithms::AlgorithmParameterName FormatString;
};
}}}
#endif
