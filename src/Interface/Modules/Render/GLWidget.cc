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

/// \author James Hughes
/// \date   September 2012
/// \brief  Not sure this file should go in Modules/Render. But it is an
///         auxiliary file to the ViewScene render module.

#include <Interface/Modules/Render/GLWidget.h>
#include <iostream>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QtDebug>
#include <Core/Application/Application.h>
#include <QGLFramebufferObjectFormat>
#include <QGLFunctions>


namespace SCIRun {
namespace Gui {

const int RendererUpdateInMS = 35;
const double updateTime = RendererUpdateInMS / 1000.0;

//------------------------------------------------------------------------------
GLWidget::GLWidget(QtGLContext* context, QWidget* parent) :
    QGLWidget(context, parent),
    mContext(new GLContext(this)),
    mCurrentTime(0.0)
{
  /// \todo Implement this intelligently. This function is called everytime
  ///       there is a new graphics context.
  std::vector<std::string> shaderSearchDirs;

  mContext->makeCurrent();

  // Call gl platform init.
  CPM_GL_PLATFORM_NS::glPlatformInit();
  
  auto frameInitLimitFromCommandLine = Core::Application::Instance().parameters()->frameInitLimit();
  if (frameInitLimitFromCommandLine)
  {
    std::cout << "Renderer frame init limit changed to " << *frameInitLimitFromCommandLine << std::endl;
  }
  const int frameInitLimit = frameInitLimitFromCommandLine.get_value_or(100);

  mGraphics.reset(new Render::SRInterface(mContext, frameInitLimit ));

  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(updateRenderer()));
  mTimer->start(RendererUpdateInMS);

  // We must disable auto buffer swap on the 'paintEvent'.
  setAutoBufferSwap(false);

  initializeGL();

  if (mFBO)
    mGraphics->setFBO( mFBO );

  hdr = true;

}

//------------------------------------------------------------------------------
GLWidget::~GLWidget()
{
  // Need to inform module that the context is being destroyed.
  if (mGraphics != nullptr)
  {
    //std::cout << "Terminating spire." << std::endl;
    mGraphics.reset();
  }

  delete mFBO;
}

//------------------------------------------------------------------------------
GLint GLWidget::createShader(const char* vtx, const char* frg)
  {
    GLint prg_id = glCreateProgram();
    GLint vtx_id = glCreateShader(GL_VERTEX_SHADER);
    GLint frg_id = glCreateShader(GL_FRAGMENT_SHADER);
    GLint ok;

    glShaderSource(vtx_id, 1, &vtx, 0);
    glCompileShader(vtx_id);
    glGetShaderiv(vtx_id,GL_COMPILE_STATUS,&ok);
    if(!ok)
    {
      std::cerr << "vtx compilation failed\n";
    }

    glShaderSource(frg_id, 1, &frg, 0);
    glCompileShader(frg_id);
    glGetShaderiv(frg_id,GL_COMPILE_STATUS,&ok);
    if(!ok)
    {
      std::cerr << "frg compilation failed\n";
    }

    glAttachShader(prg_id, vtx_id);
    glAttachShader(prg_id, frg_id);
    glLinkProgram(prg_id);
    glGetProgramiv(prg_id,GL_LINK_STATUS,&ok);
    if(!ok)
    {
      std::cerr << "linking failed\n";
    }
//    printInfoLog(prg_id);

//    glUseProgram(prg_id);
    return prg_id;
  }

void GLWidget::setHDRState(int val)
{
  if (hdr)
    hdr = false;
  else
    hdr = true;

  mGraphics->setHDRState(val);
}

void GLWidget::setGamma(double val)
{
  gamma = val;
  mGraphics->setGamma(val);
}

void GLWidget::setExposure(double val)
{
  exposure = val;
  mGraphics->setExposure(val);
}

//------------------------------------------------------------------------------
void GLWidget::initializeGL()
{

  // Set up floating point framebuffer to render scene to
/*
  GLuint hdrFBO;
  glGenFramebuffers(1, &hdrFBO);
  // - Create floating point color buffer
  GLuint colorBuffer;
  glGenTextures(1, &colorBuffer);
  glBindTexture(GL_TEXTURE_2D, colorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // - Create depth buffer (renderbuffer)
  GLuint rboDepth;
  glGenRenderbuffers(1, &rboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
  // - Attach buffers
  glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
*/

  // create our floating point framebuffer object for HDR

//  std::cout << mGraphics->getScreenWidthPixels() << std::endl;
//  std::cout << mGraphics->getScreenHeightPixels() << std::endl;

  int SCR_WIDTH = mGraphics->getScreenWidthPixels();
  int SCR_HEIGHT = mGraphics->getScreenHeightPixels();


  mFBO = new QGLFramebufferObject( SCR_WIDTH,
                                   SCR_HEIGHT,
                                   QGLFramebufferObject::Depth,
                                   GL_TEXTURE_2D,
                                   GL_RGBA16F );


  // Set up floating point framebuffer to render scene to
#if 0
  GLuint hdrFBO;
  glGenFramebuffers(1, &hdrFBO);
  // - Create floating point color buffer
  GLuint colorBuffer;
  glGenTextures(1, &colorBuffer);
  glBindTexture(GL_TEXTURE_2D, colorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mGraphics->getScreenWidthPixels(),
      mGraphics->getScreenHeightPixels(), 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // - Create depth buffer (renderbuffer)
  GLuint rboDepth;
  glGenRenderbuffers(1, &rboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
  // - Attach buffers
  glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  std::cout << hdrFBO << std::endl;
  std::cout << colorBuffer << std::endl;
#endif

  bool success = mFBO->bind();

  QGLFunctions GLfuncs( this->context() );

//  std::cout << mFBO->handle() << std::endl;

//  std::cout << success << std::endl;

  // 2. Now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
 //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //  hdrShader.Use();

     std::string vtx("#version 330 core\n"
                     "in vec3 position;\n"
                     "in vec2 texcoords;\n"
         "//in vec2 texCoords; \n"
         "out vec2 TexCoords;\n "
         "void main()\n "
         "{\n "
         "gl_Position = vec4(position.xyz, 1.0); \n "
         "TexCoords = texcoords; \n"
         "}");

/* from A. Kuhn & N. Lindow & T. Günther & A. Wiebel & H. Theisel & H.-C. Hege
   Density-based Trajectory Rendering

     The Function maps color values component-wise from
     [0,∞) to [0,1] by 1−e
     et·v
     , where v is a channel of the color
     and et is a negative parameter steering the strength of the
     mapping. For the subtractive blending, we have to apply a
     pre-mapping, because the color channels lie in the range of
     (−∞,1]. Finally, a gamma correction is necessary to avoid
     color changes by the tone mapping. In summary, our rendering
     contains four interactive parameters: A global line width
     scale rs, an alpha blending fall-off parameter e f
     , a tone mappign
     parameter et , and eg for the gamma correction. Depending
     on rs and e f both parameters adjust the resulting brightness
     and contrast.
*/

     std::string frag("#version 330 core\n"
              "out vec4 color;\n"
              "in vec2 TexCoords; uniform sampler2D hdrBuffer;\n "
              "uniform float exposure; uniform bool hdr;\n"
              "uniform float gamma;\n "
              "void main(){\n "
              "vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb\n;"
              "//color = vec4(hdrColor, 1.0);\n"
              "//vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;\n "
              "// reinhard\n"
              "//vec3 result = hdrColor / (hdrColor + vec3(1.0));\n"
              "// exposure\n"
              "vec3 result = vec3(1.0) - exp(-hdrColor * exposure);\n "
              "// also gamma correct while we're at it\n"
              "result = pow( result, vec3(1.0 / gamma) );\n "
              "//result = vec3(TexCoords.t, TexCoords.s,0); \n"
              "color = vec4(result.rgb, 1.0);\n"
              "}");

     mToneMapShaders = createShader(vtx.c_str(), frag.c_str());

     mGraphics->setShader( mToneMapShaders );

     // Bind shaders
     glUseProgram(mToneMapShaders);

//     GLboolean hdr = true; // Change with 'Space'
//     GLfloat exposure = 5.0f; // Change with Q and E

//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, colorBuffer);
     glUniform1i(glGetUniformLocation(mToneMapShaders, "hdr"), true);
     glUniform1f(glGetUniformLocation(mToneMapShaders, "exposure"), float(exposure));
     glUniform1f(glGetUniformLocation(mToneMapShaders, "gamma"), float(gamma));
     glUniform1f(glGetUniformLocation(mToneMapShaders, "hdrBuffer"), mFBO->texture());
}

//------------------------------------------------------------------------------
SCIRun::Render::SRInterface::MouseButton GLWidget::getSpireButton(QMouseEvent* event)
{
  SCIRun::Render::SRInterface::MouseButton btn = SCIRun::Render::SRInterface::MOUSE_NONE;
  if (event->buttons() & Qt::LeftButton)
    btn = Render::SRInterface::MOUSE_LEFT;
  else if (event->buttons() & Qt::RightButton)
    btn = Render::SRInterface::MOUSE_RIGHT;
  else if (event->buttons() & Qt::MidButton)
    btn = Render::SRInterface::MOUSE_MIDDLE;

  return btn;
}

//------------------------------------------------------------------------------
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
  // Extract appropriate key.
  SCIRun::Render::SRInterface::MouseButton btn = getSpireButton(event);
  mGraphics->inputMouseMove(glm::ivec2(event->x(), event->y()), btn);
  event->ignore();
}

//------------------------------------------------------------------------------
void GLWidget::mousePressEvent(QMouseEvent* event)
{
 
  SCIRun::Render::SRInterface::MouseButton btn = getSpireButton(event);
  mGraphics->inputMouseDown(glm::ivec2(event->x(), event->y()), btn);
  event->ignore();
}

//------------------------------------------------------------------------------
void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
  SCIRun::Render::SRInterface::MouseButton btn = getSpireButton(event);
  mGraphics->inputMouseUp(glm::ivec2(event->x(), event->y()), btn);
  event->ignore();
}

//------------------------------------------------------------------------------
void GLWidget::wheelEvent(QWheelEvent * event)
{
  mGraphics->inputMouseWheel(event->delta());
  event->ignore();
}

//------------------------------------------------------------------------------
void GLWidget::keyPressEvent(QKeyEvent* event)
{
  std::cout << "key down" << std::endl;
  mGraphics->inputShiftKeyDown(event->key() == Qt::Key_Shift);
  event->ignore();
}

//------------------------------------------------------------------------------
void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
  mGraphics->inputShiftKeyDown(false);
  event->ignore();
}

//------------------------------------------------------------------------------
void GLWidget::resizeGL(int width, int height)
{
  mGraphics->eventResize(static_cast<size_t>(width),
                         static_cast<size_t>(height));

  delete mFBO;

  mFBO = new QGLFramebufferObject( static_cast<size_t>(width),
                                   static_cast<size_t>(height),
                                   QGLFramebufferObject::Depth,
                                   GL_TEXTURE_2D,
                                   GL_RGBA16F );

  mGraphics->setFBO( mFBO );

  std::cout << mFBO->handle() << std::endl;

}

//------------------------------------------------------------------------------
void GLWidget::closeEvent(QCloseEvent *evt)
{
  //qDebug() << "Close event for window.";
  if (mGraphics != nullptr)
  {
    //std::cout << "Terminating spire." << std::endl;
    mGraphics.reset();
  }
  QGLWidget::closeEvent(evt);
}

//------------------------------------------------------------------------------
void GLWidget::makeCurrent()
{
  mContext->makeCurrent();
}

//------------------------------------------------------------------------------
void GLWidget::updateRenderer()
{
  mCurrentTime += updateTime;

  try
  {
//    bool success = mFBO->bind();
    mGraphics->doFrame(mCurrentTime, updateTime);


    // draw screen-aligned quad
//    GL( glViewport(0,0, mGraphics->getScreenWidthPixels(), mGraphics->getScreenWidthPixels() ) );

/*
    GL(glMatrixMode(GL_PROJECTION));
    GL(glLoadIdentity());
    GL(glOrtho(0,1,0,1,0,1));
    GL(glMatrixMode(GL_MODELVIEW));

    GL( glColor3f(1, 1,1) );
    GL(glBegin(GL_QUADS));
    GL(glVertex2f(-1,-1));
    GL(glVertex2f(1,-1));
    GL(glVertex2f(1,1));
    GL(glVertex2f(-1,1));
    GL(glEnd());
    GL(glFlush());
*/

/*
    // The fullscreen quad's FBO
    GLuint quad_VertexArrayID;
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);

    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };

    GLuint quad_vertexbuffer;
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, mGraphics->getScreenWidthPixels(), mGraphics->getScreenWidthPixels() );
*/
    mContext->swapBuffers();
  }
  catch (const SCIRun::Render::SRInterfaceFailure& e)
  {
    Q_EMIT fatalError(e.what());
    mTimer->stop();
  }
}

} // namespace Gui
} // namespace SCIRun
