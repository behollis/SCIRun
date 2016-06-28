
#ifndef MODULES_VISUALIZATION_GENERATEDENSITYPROJECTIONSTREAMLINES_H
#define MODULES_VISUALIZATION_GENERATEDENSITYPROJECTIONSTREAMLINES_H
#include <Dataflow/Network/Module.h>
//#include <Modules/Fields/share.h>
#include <Core/Thread/Interruptible.h>
#include <Modules/Legacy/Visualization/share.h>

namespace SCIRun {
  namespace Modules {
    namespace Visualization {
      class SCISHARE GenerateDensityProjectionStreamLines : public SCIRun::Dataflow::Networks::Module,
//      public HasNoInputPorts,
//      public Has1OutputPort<StringPortTag>
      public Has2InputPorts<FieldPortTag, FieldPortTag>,
      public Has1OutputPort<FieldPortTag>,
      public Core::Thread::Interruptible
      {
      public:
        GenerateDensityProjectionStreamLines();
        virtual void execute() override;
        virtual void setStateDefaults() override;
        static const Dataflow::Networks::ModuleLookupInfo staticInfo_;

        INPUT_PORT(0, Vector_Field, LegacyField);
        INPUT_PORT(1, Seed_Points, LegacyField);
        OUTPUT_PORT(0, Streamlines, LegacyField);
      };
    }
  }
}
#endif

#ifndef MODULES_LEGACY_VISUALIZATION_GENERATESTREAMLINES_H_
#define MODULES_LEGACY_VISUALIZATION_GENERATESTREAMLINES_H_

#if 0
#include <Dataflow/Network/Module.h>
#include <Core/Thread/Interruptible.h>
#include <Modules/Legacy/Visualization/share.h>

namespace SCIRun {
  namespace Modules {
    namespace Visualization {

      class SCISHARE GenerateStreamLines : public Dataflow::Networks::Module,
        public Has2InputPorts<FieldPortTag, FieldPortTag>,
        public Has1OutputPort<FieldPortTag>,
        public Core::Thread::Interruptible
      {
      public:
        GenerateStreamLines();
        virtual void setStateDefaults() override;
        virtual void execute() override;

        INPUT_PORT(0, Vector_Field, LegacyField);
        INPUT_PORT(1, Seed_Points, LegacyField);
        OUTPUT_PORT(0, Streamlines, LegacyField);

        static const Dataflow::Networks::ModuleLookupInfo staticInfo_;
      };
    }
  }
};

#endif
#endif
