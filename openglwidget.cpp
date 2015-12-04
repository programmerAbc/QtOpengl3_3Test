#include "openglwidget.h"

#include<QDebug>
#include<QDateTime>
#include<QFile>
#include <stdio.h>
#include <QOpenGLShaderProgram>
#define VERTEX_DATA 0
#define NORMAL_DATA 1
#define TEXTURE_DATA 2
#define INDEX_DATA 3
#define COLOR_DATA 4

#define VERTEX_BUFFER_NUM 5
#define VERTEX_ARRAY_OBJECT_NUM 2
#define TEXTURE_BUFFER_NUM 2
#define SQUARE_NUM 1

#define TEXTURE_WIDTH 512
#define TEXTURE_HEIGHT 512

#define PIXEL_BUFFER_SIZE TEXTURE_WIDTH*TEXTURE_HEIGHT//786432 //PIXEL_BUFFER_SIZE=TEXTURE_WIDTH*TEXTURE_HEIGHT

#define ORTHO_RANGE 1
#define SCALE_NUM 100
GLuint VBO[VERTEX_BUFFER_NUM];
GLuint VAO[VERTEX_ARRAY_OBJECT_NUM];
GLuint VBO_SCALE[VERTEX_BUFFER_NUM];
GLuint TO;
GLuint PIXEL_BUFFER[PIXEL_BUFFER_SIZE];
GLfloat SQUARE_TRANSLATE[3];
QImage image(":/pic/teximage");
GLchar* fragmentShaderSource=
        "#version 330\n"
        "uniform float red;\n"
        "uniform sampler2D textureImg;\n"
        "in vec2 outTexCoords;\n"
        "in float diff;\n"
        "out vec4 vFragColor;\n"
        "void main() {\n"
        "float delta=red;\n"
        "vec4 c00=texture(textureImg,vec2(outTexCoords.s-delta,outTexCoords.t+delta));\n"
        "vec4 c01=texture(textureImg,vec2(outTexCoords.s,outTexCoords.t-delta)).rgba*2;\n"
        "vec4 c02=texture(textureImg,vec2(outTexCoords.s+delta,outTexCoords.t+delta));\n"
        "vec4 c10=texture(textureImg,vec2(outTexCoords.s-delta,outTexCoords.t)).rgba*2;\n"
        "vec4 c11=texture(textureImg,outTexCoords.st);\n"
        "vec4 c12=texture(textureImg,vec2(outTexCoords.s+delta,outTexCoords.t)).rgba*2;\n"
        "vec4 c20=texture(textureImg,outTexCoords.st-delta);\n"
        "vec4 c21=texture(textureImg,vec2(outTexCoords.s,outTexCoords.t-delta)).rgba*2;\n"
        "vec4 c22=texture(textureImg,vec2(outTexCoords.s-delta,outTexCoords.t+delta));\n"
        "vec4 outColor=(c00+c01+c02+c10+c11+c12+c20+c21+c22)/13;\n"
        "   vFragColor = vec4(diff*diff*outColor.bgr,outColor.a);\n"
        "}\0";
GLchar* vertexShaderSource=
        "#version 330\n"
        "in vec4 position;\n"
        "in vec4 normal;\n"
        "in vec2 texCoords;\n"
         "uniform vec4 light;\n"
        "uniform mat4 normalMatrix;"
        "uniform mat4 mvMatrix;\n"
        "uniform mat4 projMatrix;\n"
        "out vec2 outTexCoords;\n"
        "out float diff;\n"
        "void main(void) {\n"
        "outTexCoords=texCoords;\n"
        "vec4 fragNormal=normalize(normalMatrix*vec4(normal.xyz,0));\n"
        "diff=dot(fragNormal,vec4(light.xyz,0));\n"
        "diff=max(0,-diff);\n"
        "gl_Position=projMatrix*mvMatrix*vec4(position.xyz,1);\n"
        "}\0";
QOpenGLShaderProgram* m_program;
GLint locRed;
GLint locMvMat;
GLint locProjMat;
GLint locTextureImg;
GLint locNormalMatrix;
GLint locLight;

QSurfaceFormat surfaceFormat;
QMatrix4x4 m_proj;
QMatrix4x4 m_mv;
OpenglWidget::OpenglWidget(QWidget *parent): QOpenGLWidget(parent)
{

    for(int i=0;i<PIXEL_BUFFER_SIZE;++i)
    {
        PIXEL_BUFFER[i]=0xFF00FF00;
    }

    surfaceFormat.setDepthBufferSize(24);
    surfaceFormat.setSwapInterval(0);
    this->setFormat(surfaceFormat);
}

void OpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();

    /*
         3-------2
         |  \        |
         |     \     |
         |        \  |
         0-------1
         */

//    GLfloat pVerts[4][3]={{-ORTHO_RANGE,-ORTHO_RANGE,0},{ORTHO_RANGE,-ORTHO_RANGE,0},{ORTHO_RANGE,ORTHO_RANGE,0},{-ORTHO_RANGE,ORTHO_RANGE,0}};
//    GLfloat pVertsScale[4][3]={{0,0,-0.1},{ORTHO_RANGE,0,-0.1},{ORTHO_RANGE,ORTHO_RANGE/2.0,-0.1},{0,ORTHO_RANGE/2.0,-0.1}};
//    GLfloat pColors[4][4]={{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1}};
//    GLfloat pNorms[4][3] = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 }, { 0, 0, 1 } };
//    GLfloat pTexCoords[4][2] ={ { 0, 1 },{  1,1 },{ 1, 0 },{ 0, 0 } };
//    GLushort pIndexes[6] = {0,1,3,1,2,3};
    GLfloat pVerts[24][3]={
        {-1,-1,1},{-1,-1,1},{-1,-1,1},    //0,1,2
        {1,-1,1},{1,-1,1},{1,-1,1},        //3,4,5
        {1,1,1},{1,1,1},{1,1,1},            //6,7,8
        {-1,1,1},{-1,1,1},{-1,1,1},        //9,10,11
        {-1,-1,-1},{-1,-1,-1},{-1,-1,-1},//12,13,14
        {1,-1,-1},{1,-1,-1},{1,-1,-1},    //15,16,17
        {1,1,-1},{1,1,-1},{1,1,-1},        //18,19,20
        {-1,1,-1},{-1,1,-1},{-1,1,-1}     //21,22,23
    };
    GLfloat pColors[24][4]={{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1},{1,0.5,0,1}};
    GLfloat pNorms[24][3] = {
        { 0, 0, 1 },//0
        { -1, 0, 0 },//1
        { 0, -1, 0 },//2
        { 0, 0, 1 },//3
        { 1, 0, 0 },//4
        { 0, -1, 0 },//5
        { 0, 0, 1 },//6
        { 1, 0, 0 },//7
        { 0, 1, 0 },//8
        { 0, 0, 1 },//9
        { -1, 0, 0 },//10
        { 0, 1, 0 },//11
        { 0, 0, -1 },//12
        { -1, 0, 0 },//13
        { 0, -1, 0 },//14
        { 0, 0, -1 },//15
        { 1, 0, 0 },//16
        { 0, -1, 0 },//17
        { 0, 0, -1 },//18
        { 1, 0, 0 },//19
        { 0, 1, 0 },//20
        { 0, 0, -1 },//21
        {-1, 0, 0 },//22
        { 0, 1, 0 },//23
    };
    GLfloat pTexCoords[24][2] ={
        { 0, 0 },//0
        { 0, 0 },//1
        { 0, 0 },//2
        { 1, 0 },//3
        { 0, 0 },//4
        { 1, 0 },//5
        { 1, 1 },//6
        { 0, 1 },//7
        { 1, 0 },//8
        { 0, 1 },//9
        { 0, 1 },//10
        { 0, 0 },//11
        { 0, 0 },//12
        { 1, 0 },//13
        { 0, 1 },//14
        { 1, 0 },//15
        { 1, 0 },//16
        { 1, 1 },//17
        { 1, 1 },//18
        { 1, 1 },//19
        { 1, 1 },//20
        { 0, 1 },//21
        { 1, 1 },//22
        { 0, 1 } //23
    };
    GLushort pIndexes[36] = {0,3,9,3,6,9,
                                            12,21,15,15,21,18,
                                            1,10,13,13,10,22,
                                            4,16,19,4,19,7,
                                            11,8,23,8,20,23,
                                            2,14,5,5,14,17};
    GLfloat vLight[4]={0,0,-1,0};
    glGenVertexArrays(2,VAO);

    glBindVertexArray(VAO[0]);
    glGenBuffers(VERTEX_BUFFER_NUM,VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 3, pVerts,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 3, pNorms,GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[TEXTURE_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 2, pTexCoords,GL_STATIC_DRAW);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[COLOR_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 4, pColors,GL_STATIC_DRAW);
    glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 36, pIndexes,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    ////////
//    glBindVertexArray(VAO[1]);
//    glGenBuffers(VERTEX_BUFFER_NUM,VBO_SCALE);

//    glBindBuffer(GL_ARRAY_BUFFER, VBO_SCALE[VERTEX_DATA]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, pVertsScale,GL_STATIC_DRAW);
//    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

//    glBindBuffer(GL_ARRAY_BUFFER, VBO_SCALE[NORMAL_DATA]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, pNorms,GL_STATIC_DRAW);
//    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

//    glBindBuffer(GL_ARRAY_BUFFER, VBO_SCALE[TEXTURE_DATA]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, pTexCoords,GL_STATIC_DRAW);
//    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,0);

//    glBindBuffer(GL_ARRAY_BUFFER, VBO_SCALE[COLOR_DATA]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, pColors,GL_STATIC_DRAW);
//    glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,0,0);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_SCALE[INDEX_DATA]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, pIndexes,GL_STATIC_DRAW);

//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glEnableVertexAttribArray(2);
//    glEnableVertexAttribArray(3);
    ////////
    glBindVertexArray(0);

    glGenTextures(1,&TO);

    glBindTexture(GL_TEXTURE_2D, TO);
    // glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,TEXTURE_WIDTH,TEXTURE_HEIGHT, 0,GL_RGBA,GL_UNSIGNED_BYTE,PIXEL_BUFFER);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,image.width(),image.height(), 0,GL_RGBA,GL_UNSIGNED_BYTE,image.constBits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_FALSE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D,0);
    glClearColor(0.5, 0.1, 0.2, 1);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("position", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->bindAttributeLocation("texCoords", 2);
    m_program->link();
    locRed=m_program->uniformLocation("red");
    locMvMat=m_program->uniformLocation("mvMatrix");
    locProjMat=m_program->uniformLocation("projMatrix");
    locTextureImg=m_program->uniformLocation("textureImg");
    locNormalMatrix=m_program->uniformLocation("normalMatrix");
    locLight=m_program->uniformLocation("light");
    m_program->bind();
    glUniform1i(locTextureImg,0);
    glUniform4fv(locLight,1,vLight);
    m_mv.setToIdentity();
}

void OpenglWidget::resizeGL(int w, int h)
{
    if(w<=0)
        w=1;
    if(h<=0)
        h=1;
    glViewport(0, 0,w, h);
    m_proj.setToIdentity();
    m_proj.perspective(15.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
    //m_proj.ortho(-2,2,-2,2,1,100);
    glUniformMatrix4fv(locProjMat,1,GL_FALSE,m_proj.data());
}

void OpenglWidget::paintGL()
{
    static float tempRed=0;
    static float delta=0.001;
    static uint time=QDateTime::currentDateTime().toTime_t();
    static int rotateDegree=0;
    static int frames=0;
    if(QDateTime::currentDateTime().toTime_t()-time>=2)
    {
        this->setWindowTitle(QString::number(frames/2));
        frames=0;
        time=QDateTime::currentDateTime().toTime_t();
    }
    frames++;
    if(tempRed>1)
    {
        delta=-0.001;
    }
    else
    {
        if(tempRed<0)
        {
            delta=0.001;
        }
    }
    glBindTexture(GL_TEXTURE_2D, TO);
    tempRed+=delta;
    m_mv.setToIdentity();
    m_mv.translate(0,0,-15);
    m_mv.rotate(rotateDegree++,1,0,1);
    glUniformMatrix4fv(locMvMat,1,GL_FALSE,m_mv.data());
    glUniformMatrix4fv(locNormalMatrix,1,GL_FALSE,m_mv.data());
    glUniform1f(locRed,tempRed);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,0);
}

