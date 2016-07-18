/// \author Brad Hollister
/// \date   July 2016

#ifndef INTERFACE_MODULES_RENDER_GLFRAMEBUFFEROBJECT_H
#define INTERFACE_MODULES_RENDER_GLFRAMEBUFFEROBJECT_H

#include <QGLFramebufferObject>

/*
http://doc.qt.io/qt-5/qglframebufferobject.html
This class is obsolete. It is provided to keep old source code working.
We strongly advise against using it in new code.

Note: This class has been deprecated in favor of QOpenGLFramebufferObject.
http://doc.qt.io/qt-5/qopenglframebufferobject.html
*/

namespace SCIRun {
namespace Gui {

///
class QtGLFramebufferObject : public QGLFramebufferObject
{
public:
  QtGLFramebufferObject(int width, int height, GLenum target);
  virtual ~QtGLFramebufferObject();

private:
};

} // namespace Gui
} // SCIRun

#endif
