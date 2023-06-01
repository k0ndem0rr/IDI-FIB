// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  projectTransform();
  viewTransform();

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pintem el Patricio
  if (!showCubs) {
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  else {
    // Pintem el cub
    glBindVertexArray(VAO_Cub);
    modelTransformCub (4.0, angleCubs+0.0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(VAO_Cub);
    modelTransformCub (5.0, angleCubs+120.0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(VAO_Cub);
    modelTransformCub (6.0, angleCubs+240.0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  ExamGLWidget::modelTransformCub (1.0, 0.0);
  // En aquest mètode has de substituir aquest codi per construir la 
  // transformació geomètrica (TG) del cub usant els paràmetres adientment

  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::scale(TG, glm::vec3(escala));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);

  TG = glm::rotate(TG, glm::radians(angleCubs+anglePatricio), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::rotate(TG, glm::radians(-90.f), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2));
  TG = glm::scale(TG, glm::vec3 (escala, escala, escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
   View =glm::lookAt(glm::vec3(0,2*radiEsc,0), centreEsc, glm::vec3(1,0,0));

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    if (ra > 1) Proj = glm::ortho(-radiEsc*ra, radiEsc*ra, -radiEsc, radiEsc, zn, zf);
    else Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc/ra, radiEsc/ra, zn, zf);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      showCubs = !showCubs;
    break;
	}
  case Qt::Key_1: {
      anglePatricio = 0;
      emit caixaCanviada(1);
    break;
	}
  case Qt::Key_2: {
      anglePatricio = 120;
      emit caixaCanviada(2);
    break;
	}
  case Qt::Key_3: {
      anglePatricio = 240;
      emit caixaCanviada(3);
    break;
	}
  case Qt::Key_F: {
      colFoc.z = !colFoc.z;
      enviaColFocus();
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_Right: {
      angleCubs -= 120;
    break;
	}
  case Qt::Key_Left: {
      angleCubs += 120;
    break;
	}
  case Qt::Key_R: {
      iniEscena();
      iniCamera();
      emit caixaCanviada(1);
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::iniEscena ()
{
  ExamGLWidget::iniEscena();
  showCubs = true;
  anglePatricio = 0;
  angleCubs = 0;
  camPlanta = false;
}

void MyGLWidget::canviCamara()
{
  makeCurrent();
  camPlanta = !camPlanta;
  update();
}

void MyGLWidget::caixa(int i)
{
  makeCurrent();
  anglePatricio = (i-1)*120;
  update();
}