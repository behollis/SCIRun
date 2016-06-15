#include <Modules/String/TestModuleSimpleUI.h>
#include <Core/Datatypes/String.h>

using namespace SCIRun;
using namespace SCIRun::Modules::StringManip;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Dataflow::Networks;
/// @class TestModuleSimpleUI
/// @brief This module splits out a string.

const ModuleLookupInfo TestModuleSimpleUI::staticInfo_("TestModuleSimpleUI",
"String", "SCIRun");
TestModuleSimpleUI::TestModuleSimpleUI() : Module(staticInfo_)
{
  INITIALIZE_PORT(OutputString);
}

void
TestModuleSimpleUI::execute()
{
  std::string message_string;
  message_string = "[Personalize your message here.]";
  StringHandle msH(new String(message_string));
  sendOutput(OutputString, msH);
}
