/// \author Brad Hollister
/// \date   July 2016

#include "QtGLFramebufferObject.h"

namespace SCIRun {
  namespace Gui {

    QtGLFramebufferObject::QtGLFramebufferObject(int width, int height, GLenum target) :
    QGLFramebufferObject(int width, int height, GLenum target)
    {
    }

    QtGLFramebufferObject::~QtGLFramebufferObject()
    {
    }
  }
}  // end of namespace SCIRun::Gui
