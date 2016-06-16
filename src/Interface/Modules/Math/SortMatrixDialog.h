#ifndef INTERFACE_MODULES_MATH_SortMatrixDIALOG_H
#define INTERFACE_MODULES_MATH_SortMatrixDIALOG_H 1

#include "Interface/Modules/Math/ui_SortMatrixDialog.h"
#include <Interface/Modules/Base/ModuleDialogGeneric.h>
#include <Interface/Modules/Math/share.h>

namespace SCIRun {
  namespace Gui {
    class SCISHARE SortMatrixDialog : public ModuleDialogGeneric,
    public Ui::SortMatrixDialog
    {
      Q_OBJECT

      public:
        SortMatrixDialog(const std::string& name, SCIRun::Dataflow::Networks::ModuleStateHandle state,
          QWidget* parent = 0);
    };
  }
}
#endif
