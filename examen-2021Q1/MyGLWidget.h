#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void iniEscena ();

    float angleCubs;
    float anglePatricio;
    bool showCubs;

  private:
    int printOglError(const char file[], int line, const char func[]);

  public slots:
    void canviCamara();
    void caixa(int i);

  signals:
    void caixaCanviada(int i);
};
