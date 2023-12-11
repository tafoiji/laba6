#include "letter.h"
#include "ui_mainwindow.h"
#include <iostream>
GLfloat VertexArray[64][3];
//GLubyte IndexArray[13][4];
GLubyte IndexArray[30][4] = {
    {0,10,6,9},
    {0,10,11,1},
    {1,11,12,2},
    {2,12,13,3},
    {3,13,14,4},
    {4,14,15,5},
    {6,7,8,9},

    //+16
    {16,26,22,25},
    {16,26,27,17},
    {17,27,28,18},
    {18,28,29,19},
    {19,29,30,20},
    {20,30,31,21},
    {22,23,24,25},

    //edges
    {0,16,25,9},
    {25,9,8,24},
    {24,8,7,23},
    {23,7,6,22},
    {22,6,5,21},
    {21,5,4,20},
    {20,4,3,19},
    {19,3,2,18},
    {18,2,1,17},
    {17,1,0,16},

    {10,26,27,11},
    {11,27,28,12},
    {12,28,29,13},
    {13,29,30,14},
    {14,30,31,15},
    {15,31,26,10}

};

letter::letter(QWidget *parent)
    : QGLWidget(parent)
{
    setGeometry(400,200,800,600);
    // setFormat(QGLFormat(QGL::DepthBuffer)); // использовать буфер глубины
    // установлено по умолчанию в контексте

    // начальные значения
    xRot=0; yRot=0; zRot=0; zTra=-1; nSca=1;

    // передает дальше указатель на объект pwgt
}

void letter::initializeGL()
{
    qglClearColor(Qt::white); // цвет для очистки буфера изображения - здесь просто фон окна
    glEnable(GL_DEPTH_TEST);  // устанавливает режим проверки глубины пикселей
    glShadeModel(GL_FLAT);    // отключает режим сглаживания цветов
    glEnableClientState(GL_VERTEX_ARRAY); // активизация массива вершин
}

void letter::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION); // устанавливает текущей проекционную матрицу
    glLoadIdentity();            // присваивает проекционной матрице единичную матрицу

    GLfloat ratio=(GLfloat)nHeight/(GLfloat)nWidth; // отношение высоты окна виджета к его ширине

    // мировое окно
    if (nWidth>=nHeight)
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -5.0, 5.0); // параметры видимости ортогональной проекции
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -5.0, 5.0); // параметры видимости ортогональной проекции
    // плоскости отсечения (левая, правая, верхняя, нижняя, передняя, задняя)

    // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0); // параметры видимости перспективной проекции
    // плоскости отсечения (левая, правая, верхняя, нижняя, ближняя, дальняя)

    // поле просмотра
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void letter::paintGL()
{
    glClearColor(1,1,1,0); // цвет фона
    glClear(GL_COLOR_BUFFER_BIT); // окно виджета очищается текущим цветом очистки
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка буфера изображения и глубины
    glMatrixMode(GL_MODELVIEW); // устанавливает положение и ориентацию матрице моделирования
    glLoadIdentity();           // загружает единичную матрицу моделирования
    // последовательные преобразования
    glScalef(nSca, nSca, nSca);        // масштабирование
    glTranslatef(0.0f, zTra, 0.0f);  // трансляция
    glRotatef(xRot, 1.0f, 0.0f, 0.0f); // поворот вокруг оси X
    glRotatef(yRot, 0.0f, 1.0f, 0.0f); // поворот вокруг оси Y
    glRotatef(zRot, 0.0f, 0.0f, 1.0f); // поворот вокруг оси Z
    drawAxis();
    double e=0,r=0,t=0;
    colour.getRgbF(&e,&r,&t,nullptr);
    glColor4f(e, r, t, 0.0f);
    getVertexArray(0.8);
    //getIndexArray();
    if(check==1)
        draw_xy_projection();
    else if(check==2)
        draw_xz_projection();
    else if(check==3)
        draw_zy_projection();
    else if(check==4)
        draw_scaling();
    else if(check==5)
        draw_transfer();
    else if(check==6)
        draw_rotation_x();
    else if(check==7)
        draw_rotation_y();
    else if(check==8)
        draw_rotation_z();
    else if(check==9)
        drawFigure();
    QFont tmpfont;
    tmpfont.setFamily("Arial Black");
    tmpfont.setPointSize(10);
    tmpfont.setBold(false);
    glColor3f(0,0,0);
    renderText(10,0.0,0.0,"X",tmpfont);
    glColor3f(0,0,0);
    renderText(0.0,10,0.0,"Y",tmpfont);
    glColor3f(0,0,0);
    renderText(0.0,0.0,10,"Z",tmpfont);

}

/*virtual*/void letter::mousePressEvent(QMouseEvent* pe) // нажатие клавиши мыши
{
    // при нажатии пользователем кнопки мыши переменной ptrMousePosition будет
    // присвоена координата указателя мыши
    ptrMousePosition = pe->pos();

    // ptrMousePosition = (*pe).pos(); // можно и так написать
}

/*virtual*/void letter::mouseReleaseEvent(QMouseEvent* pe) // отжатие клавиши мыши
{
    // некоторые функции, которые должны выполняться при отжатии клавиши мыши
}

/*virtual*/void letter::mouseMoveEvent(QMouseEvent* pe) // изменение положения стрелки мыши
{
    xRot += 1/M_PI*(GLfloat)(pe->y()-ptrMousePosition.y());//height(); // вычисление углов поворота
    yRot += 1/M_PI*(GLfloat)(pe->x()-ptrMousePosition.x());//width();
    ptrMousePosition = pe->pos();
    updateGL(); // обновление изображения
}

/*virtual*/void letter::wheelEvent(QWheelEvent* pe) // вращение колёсика мыши
{
    if ((pe->delta())>0) scale_plus(); else if ((pe->delta())<0) scale_minus();

    updateGL(); // обновление изображения
}

/*virtual*/void letter::keyPressEvent(QKeyEvent* pe) // нажатие определенной клавиши
{
    switch (pe->key())
    {
    case Qt::Key_Plus:
        scale_plus();     // приблизить сцену
        break;

    case Qt::Key_Equal:
        scale_plus();     // приблизить сцену
        break;

    case Qt::Key_Minus:
        scale_minus();    // удалиться от сцены
        break;

    case Qt::Key_W:
        rotate_up();      // повернуть сцену вверх
        break;

    case Qt::Key_S:
        rotate_down();    // повернуть сцену вниз
        break;

    case Qt::Key_A:
        rotate_left();     // повернуть сцену влево
        break;

    case Qt::Key_D:
        rotate_right();   // повернуть сцену вправо
        break;

    case Qt::Key_Z:
        translate_down(); // транслировать сцену вниз
        break;

    case Qt::Key_X:
        translate_up();   // транслировать сцену вверх
        break;

    case Qt::Key_Space:  // клавиша пробела
        defaultScene();   // возвращение значений по умолчанию
        break;

    case Qt::Key_Escape: // клавиша "эскейп"
        this->close();    // завершает приложение
        break;
    }

    updateGL(); // обновление изображения
}


void letter::scale_plus() // приблизить сцену
{
    nSca = nSca*1.1;
}

void letter::scale_minus() // удалиться от сцены
{
    nSca = nSca/1.1;
}

void letter::rotate_up() // повернуть сцену вверх
{
    xRot += 1.0;
}

void letter::rotate_down() // повернуть сцену вниз
{
    xRot -= 1.0;
}

void letter::rotate_left() // повернуть сцену влево
{
    zRot += 1.0;
}

void letter::rotate_right() // повернуть сцену вправо
{
    zRot -= 1.0;
}

void letter::translate_down() // транслировать сцену вниз
{
    zTra -= 0.05;
}

void letter::translate_up() // транслировать сцену вверх
{
    zTra += 0.05;
}

void letter::defaultScene() // наблюдение сцены по умолчанию
{
    xRot=-90; yRot=0; zRot=0; zTra=0; nSca=1;
}
void letter::drawAxis()
{
    glLineWidth(3.0f); // устанавливаю ширину линии приближённо в пикселях
    // до вызова команды ширина равна 1 пикселю по умолчанию

    glColor4f(1.00f, 0.00f, 0.00f, 1.0f); // устанавливается цвет последующих примитивов
    // ось x красного цвета
    glBegin(GL_LINES); // построение линии
    glVertex3f( 10.0f,  0.0f,  0.0f); // первая точка
    glVertex3f(-10.0f,  0.0f,  0.0f); // вторая точка
    glEnd();

    QColor halfGreen(0, 128, 0, 255);
    qglColor(halfGreen);
    glBegin(GL_LINES);
    // ось y зеленого цвета
    glVertex3f( 0.0f,  10.0f,  0.0f);
    glVertex3f( 0.0f, -10.0f,  0.0f);

    glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
    // ось z синего цвета
    glVertex3f( 0.0f,  0.0f,  10.0f);
    glVertex3f( 0.0f,  0.0f, -10.0f);

    glColor4f(0.00f, 0.00f, 0.00f, 0.0f);
    for (float i = -10; i < 10; i+=0.25){
        float k = 20.0;
        glBegin(GL_LINES);
        glVertex3f(i, -1.0 / k, 0.0f);
        glVertex3f(i, 1.0 / k, 0.0f);
        glVertex3f(-1.0 / k, i, 0.0f);
        glVertex3f(1.0 / k, i, 0.0f);
        glVertex3f( 0.0f, 1.0 / k, i);
        glVertex3f( 0.0f, -1.0 / k, i);
    }

    glEnd();
}

void letter::getVertexArray(float k)
{

    //внешний контур Б
    VertexArray[0][0]=0;
    VertexArray[0][1]=0;
    VertexArray[0][2]=0;

    VertexArray[1][0]=0.75*k;
    VertexArray[1][1]=0;
    VertexArray[1][2]=0;

    VertexArray[2][0]=k;
    VertexArray[2][1]=0.25 * k;
    VertexArray[2][2]=0;

    VertexArray[3][0]=k;
    VertexArray[3][1]=0.75*k;
    VertexArray[3][2]=0;

    VertexArray[4][0]=0.75*k;
    VertexArray[4][1]=k;
    VertexArray[4][2]=0;

    VertexArray[5][0]=0.25*k;
    VertexArray[5][1]=k;
    VertexArray[5][2]=0;

    VertexArray[6][0]=0.25*k;
    VertexArray[6][1]=1.5 * k;
    VertexArray[6][2]=0;

    VertexArray[7][0]=k;
    VertexArray[7][1]=1.5 * k;
    VertexArray[7][2]=0;

    VertexArray[8][0]=k;
    VertexArray[8][1]=1.75*k;
    VertexArray[8][2]=0;

    VertexArray[9][0]=0;
    VertexArray[9][1]=1.75*k;
    VertexArray[9][2]=0;

    //внутренний контур Б

    VertexArray[10][0]=0.25*k;
    VertexArray[10][1]=0.25*k;
    VertexArray[10][2]=0;

    VertexArray[11][0]=0.625 * k;
    VertexArray[11][1]=0.25 * k;
    VertexArray[11][2]=0;

    VertexArray[12][0]=0.75*k;
    VertexArray[12][1]=0.375*k;
    VertexArray[12][2]=0;

    VertexArray[13][0]=0.75*k;
    VertexArray[13][1]=0.625*k;
    VertexArray[13][2]=0;

    VertexArray[14][0]=0.625*k;
    VertexArray[14][1]=0.75*k;
    VertexArray[14][2]=0;

    VertexArray[15][0]=0.25*k;
    VertexArray[15][1]=0.75*k;
    VertexArray[15][2]=0;

    //внешний контур Б
    VertexArray[16][0]=0;
    VertexArray[16][1]=0;
    VertexArray[16][2]=0.25*k;

    VertexArray[17][0]=0.75*k;
    VertexArray[17][1]=0;
    VertexArray[17][2]=0.25*k;

    VertexArray[18][0]=k;
    VertexArray[18][1]=0.25 * k;
    VertexArray[18][2]=0.25*k;

    VertexArray[19][0]=k;
    VertexArray[19][1]=0.75*k;
    VertexArray[19][2]=0.25*k;

    VertexArray[20][0]=0.75*k;
    VertexArray[20][1]=k;
    VertexArray[20][2]=0.25*k;

    VertexArray[21][0]=0.25*k;
    VertexArray[21][1]=k;
    VertexArray[21][2]=0.25*k;

    VertexArray[22][0]=0.25*k;
    VertexArray[22][1]=1.5 * k;
    VertexArray[22][2]=0.25*k;

    VertexArray[23][0]=k;
    VertexArray[23][1]=1.5 * k;
    VertexArray[23][2]=0.25*k;

    VertexArray[24][0]=k;
    VertexArray[24][1]=1.75*k;
    VertexArray[24][2]=0.25*k;

    VertexArray[25][0]=0;
    VertexArray[25][1]=1.75*k;
    VertexArray[25][2]=0.25*k;

    //внутренний контур Б

    VertexArray[26][0]=0.25*k;
    VertexArray[26][1]=0.25*k;
    VertexArray[26][2]=0.25*k;

    VertexArray[27][0]=0.625 * k;
    VertexArray[27][1]=0.25 * k;
    VertexArray[27][2]=0.25*k;

    VertexArray[28][0]=0.75*k;
    VertexArray[28][1]=0.375*k;
    VertexArray[28][2]=0.25*k;

    VertexArray[29][0]=0.75*k;
    VertexArray[29][1]=0.625*k;
    VertexArray[29][2]=0.25*k;

    VertexArray[30][0]=0.625*k;
    VertexArray[30][1]=0.75*k;
    VertexArray[30][2]=0.25*k;

    VertexArray[31][0]=0.25*k;
    VertexArray[31][1]=0.75*k;
    VertexArray[31][2]=0.25*k;
}

void letter::draw_rotation_z()
{
    GLfloat c[64][3];
    GLfloat conv[3][3] ;
    conv[0][0]=cos(angle*(M_PI/180.0));;
    conv[0][1]=-sin(angle*(M_PI/180.0));;
    conv[0][2]=0;

    conv[1][0]=sin(angle*(M_PI/180.0));
    conv[1][1]=cos(angle*(M_PI/180.0));
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=1;

    QTextStream out(stdout);
    out<<"Z rotation matrix\n";
    for(int i=0; i < 3; i++)
    {
        out<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" \n";
    }

    for(int i=0; i < 64; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
            {
                c[i][j]+=VertexArray[i][k]*conv[k][j];
            }
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);

}
void letter::draw_rotation_y()
{
    GLfloat c[64][3];
    GLfloat conv[3][3] ;
    conv[0][0]=cos(angle*(M_PI/180.0));
    conv[0][1]=0;
    conv[0][2]=sin(angle*(M_PI/180.0));

    conv[1][0]=0;
    conv[1][1]=1;
    conv[1][2]=0;

    conv[2][0]=-sin(angle*(M_PI/180.0));
    conv[2][1]=0;
    conv[2][2]=cos(angle*(M_PI/180.0));

    QTextStream out(stdout);
    out<<"Y rotation matrix\n";
    for(int i=0; i < 3; i++)
    {
        out<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<"\n";
    }

    for(int i=0; i < 64; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
            {
                c[i][j]+=VertexArray[i][k]*conv[k][j];
            }
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);

}
void letter::draw_rotation_x()
{
    GLfloat c[64][3];
    GLfloat conv[3][3] ;
    conv[0][0]=1;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=cos(angle*(M_PI/180.0));
    conv[1][2]=-sin(angle*(M_PI/180.0));

    conv[2][0]=0;
    conv[2][1]=sin(angle*(M_PI/180.0));
    conv[2][2]=cos(angle*(M_PI/180.0));


    QTextStream out(stdout);
    out<<"X rotation matrix\n";
    for(int i=0; i < 3; i++)
    {
        out<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<"\n";
    }

    for(int i=0; i < 64; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
            {
                c[i][j]+=VertexArray[i][k]*conv[k][j];
            }
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);

}
void letter::draw_transfer()
{
    GLfloat c[64][3];
    GLfloat conv[4][3] ;
    conv[0][0]=1;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=1;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=1;

    conv[3][0]=x_transfer;
    conv[3][1]=y_transfer;
    conv[3][2]=z_transfer;

    QTextStream out(stdout);
    out<<"transfer matrix\n";
    for(int i=0; i < 4; i++)
    {
        out<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<"\n";
    }

    for(int i=0; i < 64; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 4; k++)
            {
                if(k==3)
                    c[i][j]+=1*conv[k][j];
                else
                    c[i][j]+=VertexArray[i][k]*conv[k][j];
            }
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::draw_scaling()
{
    GLfloat c[64][3];
    GLfloat conv[3][3] ;
    conv[0][0]=x_scale;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=y_scale;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=z_scale;

    QTextStream out(stdout);
    out<<"scale matrix\n";
    for(int i=0; i < 3; i++)
    {
        out<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<"\n";
    }

    for(int i=0; i < 64; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
                c[i][j]+=VertexArray[i][k]*conv[k][j];
        }
    }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::draw_xy_projection()
{
    GLfloat c[64][3];
    GLfloat conv[3][3] ;
    conv[0][0]=1;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=1;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=0;
    for(int i=0; i < 64; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
                c[i][j]+=VertexArray[i][k]*conv[k][j];
        }
    }

    QTextStream out(stdout);
    out<<"xy projection matrix\n";
    for(int i=0; i < 3; i++)
    {
        out<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<"\n";
    }

    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::draw_xz_projection()
{
    GLfloat c[64][3];
    GLfloat conv[3][3] ;
    conv[0][0]=1;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=0;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=1;
    for(int i=0; i < 64; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
                c[i][j]+=VertexArray[i][k]*conv[k][j];
        }
    }

    QTextStream out(stdout);
    out<<"xz projection matrix\n";
    for(int i=0; i < 3; i++)
    {
        out<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<"\n";
    }

    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::draw_zy_projection()
{
    GLfloat c[64][3];
    GLfloat conv[3][3] ;
    conv[0][0]=0;
    conv[0][1]=0;
    conv[0][2]=0;

    conv[1][0]=0;
    conv[1][1]=1;
    conv[1][2]=0;

    conv[2][0]=0;
    conv[2][1]=0;
    conv[2][2]=1;
    for(int i=0; i < 64; i++)
    {
        for(int j=0; j < 3; j++)
        {
            c[i][j]=0;
            for(int k=0; k < 3; k++)
                c[i][j]+=VertexArray[i][k]*conv[k][j];
        }
    }

    QTextStream out(stdout);
    out<<"zy projection matrix\n";
    for(int i=0; i < 3; i++)
    {
        out<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<"\n";
    }

    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);
}
void letter::drawFigure(){
    glVertexPointer(3, GL_FLOAT, 0, VertexArray);
    //  glEnableClientState(GL_VERTEX_ARRAY);
    glDrawElements(GL_QUADS, 120, GL_UNSIGNED_BYTE, IndexArray);
    //  glDisableClientState(GL_VERTEX_ARRAY);
}
