#include <Modules/String/TestModuleSimpleUI.h>
#include <Core/Datatypes/String.h>

using namespace SCIRun;
using namespace SCIRun::Modules::StringManip;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Dataflow::Networks;
/// @class TestModuleSimpleUI
/// @brief This module splits out a string.

// page 25 of SCIRun5ModuleGeneration.pdf had typo. PrintStringIntoString -> TestModuleSimple UI

SCIRun::Core::Algorithms::AlgorithmParameterName TestModuleSimpleUI::FormatString("FormatString");

const ModuleLookupInfo TestModuleSimpleUI::staticInfo_("TestModuleSimpleUI",
"String", "SCIRun");
TestModuleSimpleUI::TestModuleSimpleUI() : Module(staticInfo_)
{
  INITIALIZE_PORT(OutputString);
}

void TestModuleSimpleUI::setStateDefaults()
{
  auto state = get_state();
  state->setValue(FormatString,std::string ("[Insert message here]"));
}

void
TestModuleSimpleUI::execute()
{
  std::string message_string;
  auto state = get_state();
  message_string = state->getValue(FormatString).toString();
  StringHandle msH(new String(message_string));
  sendOutput(OutputString, msH);
}
