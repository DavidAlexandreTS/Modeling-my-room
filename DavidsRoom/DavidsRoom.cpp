#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include "SOIL.h"
#include "draw_objects.h"
#include "obj_import.h"
#include "stb_image.h"

# define M_PI           3.14159265358979323846  /* pi */

using namespace std;

GLint WIDTH = 700;
GLint HEIGHT = 700;

GLint modo = GL_MODULATE;

// Luz spot da luminaria
GLfloat luzAmbSpot[4]= {0.2,0.2,0.2,1.0};
GLfloat posLuzSpot[4] = {  260,120,65, 1 };
GLfloat dirLuzSpot[3] = { 0, -1,0 };
GLfloat luzDifusaSpot[4] = { 1,1,1,1 };
GLfloat luzEspecSpot[4] = { 0,0,0,1 };

//Configuracoes de iluminacao
float light_ambient[] = { 0.75f, 0.75f, 0.75f }; //Luz ambiente
float light_diffuse[] = { 0.0f , 0.0f , 0.5f }; //Luz azulzinha
float light_specular[] = { 1.0f, 0.0f, 0.0f }; // Luz branca

// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;

// XZ position of the camera
float x = -5.0f, z = 6.0f, y = 1.0f;

float angle = 0.0f;

float door_angle;
float wind_angle;
float angle_v = 0;

int flag_door = 0;
int flag_window = 0;
int flag_ven = 1;

GLuint texture_id[20];

void drawCube(GLdouble size, int* tex)
{
    static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--)
    {
        if (tex[i] >= 0)
        {
            glBindTexture(GL_TEXTURE_2D, texture_id[tex[i]]);
        }
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        if (i == 1)
            glTexCoord2f(1.0f, 0.0f);
        else
            glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(&v[faces[i][0]][0]);
        if (i == 1)
            glTexCoord2f(0.0f, 0.0f);
        else
            glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(&v[faces[i][1]][0]);
        if (i == 1)
            glTexCoord2f(0.0f, 1.0f);
        else
            glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(&v[faces[i][2]][0]);
        if (i == 1)
            glTexCoord2f(1.0f, 1.0f);
        else
            glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
        if (tex[i] >= 0)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void drawCircle(float r, float h, float n)
{
    int i;

    float angle = 2.0f * M_PI / n;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < n; i++)
    {
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(r * cos(i * angle), 0.0f, r * sin(i * angle));
        glVertex3f(r * cos((i + 1) * angle), 0.0f, r* sin((i + 1) * angle));
    }

    glEnd();
}

void glutSolidCylinder(float radius, float height, int sectors)
{
    int i;
    float theta = 2.0f * M_PI / sectors;
    float top = height / 2.0f;
    float bottom = -top;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < sectors; i++)
    {
        float c0 = cos(i * theta);
        float s0 = sin(i * theta);
        float c1 = cos((i + 1) * theta);
        float s1 = sin((i + 1) * theta);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, top, 0.0f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(radius * c0, top, radius * s0);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(radius * c1, top, radius * s1);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, bottom, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(radius * c1, bottom, radius * s1);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(radius * c0, bottom, radius * s0);

        glNormal3f(c0, 0.0f, s0);
        glVertex3f(radius * c0, bottom, radius * s0);
        glNormal3f(c1, 0.0f, s1);
        glVertex3f(radius * c1, top, radius * s1);
        glNormal3f(c0, 0.0f, s0);
        glVertex3f(radius * c0, top, radius * s0);

        glNormal3f(c1, 0.0f, s1);
        glVertex3f(radius * c1, top, radius * s1);
        glNormal3f(c0, 0.0f, s0);
        glVertex3f(radius * c0, bottom, radius * s0);
        glNormal3f(c1, 0.0f, s1);
        glVertex3f(radius * c1, bottom, radius * s1);
    }

    glEnd();
}

void draw_lightfix()
{
    //Base da luminaria
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(5.32f, 1.05f, -30.0);
    glScalef(0.3, 0.12, 0.3);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Cabo de baixo
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(5.32f, 1.25f, -30.0);
    glScalef(0.05, 0.5, 0.05);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Cabo de cima
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(5.5f, 1.6f, -30.0);
    glRotatef(120, 0.0, 0.0, 50.0);
    glScalef(0.05, 0.5, 0.05);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Desenha Tubo da luminaria
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(5.75f, 1.75f, -30.0);
    glScalef(0.14f, 0.3f, 0.14f);
    glutSolidCylinder(0.74f, 0.5f, 100);
    glPopMatrix();

    //Luz da luminária
    glColor3f(255, 255, 255);
    glPushMatrix();
    glTranslatef(5.75f, 1.72f, -30.0);
    glScalef(0.002f, 0.0024f, 0.002f);
    glutSolidSphere(40, 40, 40);
    glPopMatrix();
}

void drawDecorations()
{
    //Primeira pirâmide(esquerda)
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(-0.98f, 2.505f, -31.2);
    glScalef(0.25f, 0.035f, 0.25f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.98f, 2.530f, -31.2);
    glScalef(0.20f, 0.035f, 0.20f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.98f, 2.555f, -31.2);
    glScalef(0.15f, 0.035f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.98f, 2.58f, -31.2);
    glScalef(0.10f, 0.035f, 0.10f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.98f, 2.605f, -31.2);
    glScalef(0.05f, 0.035f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Segunda pirâmide(direita)
    glColor3f(0, 1, 0);
    glPushMatrix();
    glTranslatef(1.5f, 2.31f, -31.2f);
    glScalef(0.20f, 0.025f, 0.20f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.5f, 2.335f, -31.2f);
    glScalef(0.15f, 0.025f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.5f, 2.355f, -31.2f);
    glScalef(0.10f, 0.025f, 0.10f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.5f, 2.38f, -31.2f);
    glScalef(0.05f, 0.025f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.5f, 2.398f, -31.2f);
    glScalef(0.025f, 0.025f, 0.025f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Terceira pirâmide (centro)
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(0.4f, 3.115f, -31.2);
    glScalef(0.225f, 0.04f, 0.225f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4f, 3.14f, -31.2);
    glScalef(0.175f, 0.04f, 0.175f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4f, 3.165f, -31.2);
    glScalef(0.15f, 0.04f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4f, 3.19f, -31.2);
    glScalef(0.125f, 0.04f, 0.125f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4f, 3.205f, -31.2);
    glScalef(0.100f, 0.04f, 0.100f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4f, 3.230f, -31.2);
    glScalef(0.075f, 0.04f, 0.075f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4f, 3.255f, -31.2);
    glScalef(0.05f, 0.04f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4f, 3.28f, -31.2);
    glScalef(0.025f, 0.04f, 0.025f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawPoleDance()
{
    //Desenha PoleDance
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(2.55f, 2.75f, -16.4f);
    glScalef(0.008f, 0.4f, 0.008f);
    glutSolidCylinder(10.0f, 15.0f, 100);
    glPopMatrix();
}

void drawAeretor()
{
    //Suporte do ventilador
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(1.0f, 5.075f, -24.0);
    glScalef(0.05, 1.1, 0.05);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Luz do ventilador
    glColor3f(255, 255, 255);
    glPushMatrix();
    glTranslatef(1.0f, 4.4625f, -24.0f);
    glScalef(0.0045f, 0.0075f, 0.0045f);
    glutSolidSphere(40, 40, 40);
    glPopMatrix();

    //Hélice I
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(1.0f, 4.4625f, -24.0f);
    if (flag_ven == 1)
    {
        //Adicionando "-" pra girar ele n o sentido horário
        glRotatef(-(angle_v), 0, 0.9, 0);
        glTranslatef(0.00, 0.0f, -0.035);
        glScalef(0.0025f, 0.0015f, 0.025f);
        glutSolidSphere(40, 40, 40);
        glPopMatrix();
        flag_ven = 2;
        angle_v += 30;
        glutPostRedisplay();
    }
    else if (flag_ven == 2)
    {
        //Adicionando "-" pra girar ele n o sentido horário
        glRotatef(-(angle_v), 0, 0.9, 0);
        glTranslatef(0.00, 0.0f, -0.035);
        glScalef(0.0025f, 0.0015f, 0.025f);
        glutSolidSphere(40, 40, 40);
        glPopMatrix();
        flag_ven = 3;
        angle_v += 30;
        glutPostRedisplay();
    }
    else if (flag_ven == 3)
    {
        //Adicionando "-" pra girar ele n o sentido horário
        glRotatef(-(angle_v), 0, 0.9, 0);
        glTranslatef(0.00, 0.0f, -0.035);
        glScalef(0.0025f, 0.0015f, 0.025f);
        glutSolidSphere(40, 40, 40);
        glPopMatrix();
        flag_ven = 4;
        angle_v += 30;
        glutPostRedisplay();
    }
    else if (flag_ven == 4)
    {
        //Adicionando "-" pra girar ele n o sentido horário
        glRotatef(-(angle_v), 0, 0.9, 0);
        glTranslatef(0.00, 0.0f, -0.035);
        glScalef(0.0025f, 0.0015f, 0.025f);
        glutSolidSphere(40, 40, 40);
        glPopMatrix();
        flag_ven = 5;
        angle_v += 30;
        glutPostRedisplay();
    }
    else if (flag_ven == 5)
    {
        //Adicionando "-" pra girar ele n o sentido horário
        glRotatef(-(angle_v), 0, 0.9, 0);
        glTranslatef(0.00, 0.0f, -0.035);
        glScalef(0.0025f, 0.0015f, 0.025f);
        glutSolidSphere(40, 40, 40);
        glPopMatrix();
        flag_ven = 1;
        angle_v = 0;
        glutPostRedisplay();
    }

    //Removendo segunda Hélice pra o ventilador ficar mais lento
    /*Hélice II
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(1.0f, 4.4625f, -24.0f);
    glRotatef(90, 0, -40, 0);
    glTranslatef(0.00, 0.0f, -0.035);
    glScalef(0.0025f, 0.0015f, 0.025f);
    glutSolidSphere(40, 40, 40);
    glPopMatrix();*/
}

void drawPictr()
{
    //Borda inferior
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-6.94f, 3.4f, -22.0);
    glScalef(0.05f, 0.05f, 0.75f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Borda superior
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-6.94f, 4.25f, -22.0);
    glScalef(0.05f, 0.05f, 0.75f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Borda lateral esquerda
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-6.94f, 3.82f, -21.65);
    glScalef(0.05f, 0.9f, 0.054f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Borda lateral direita
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-6.94f, 3.82f, -22.4);
    glScalef(0.05f, 0.9f, 0.054f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Conteúdo do quadro
    glColor3f(0.309804f, 0.184314f, 0.309804f);
    glPushMatrix();
    glTranslatef(-6.94f, 3.82f, -22.02);
    glScalef(0.05f, 0.80f, 0.70f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawWritingBoard()
{
    //Textura da Comoda
    int wboard_texture[] = { 10, 10, 10, 10, 10, 10 };

    //Parte principal do quadro
    glColor3f(0, 0.05, 0);
    glPushMatrix();
    glTranslatef(-6.94f, 2.0f, -16.0);
    glScalef(0.25f, 2.0, 4.0);
    drawCube(1.0f, wboard_texture);
    glPopMatrix();

    //Madeira inferior do quadro
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-6.85f, 1.05f, -16.0);
    glScalef(0.5f, 0.25, 4.42);
    drawCube(1.0f, wboard_texture);
    glPopMatrix();

    //Madeira superior do quadro
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-6.94f, 3.05f, -16.0);
    glScalef(0.25f, 0.1, 4.42);
    drawCube(1.0f, wboard_texture);
    glPopMatrix();

    //Madeira Lateral esquerda
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-6.94f, 2.0f, -13.9);
    glScalef(0.25f, 2.0f, 0.22);
    drawCube(1.0f, wboard_texture);
    glPopMatrix();

    //Madeira Lateral direita
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-6.94f, 2.0f, -18.1);
    glScalef(0.25f, 2.0f, 0.22);
    drawCube(1.0f, wboard_texture);
    glPopMatrix();
}

void drawComfortable()
{
    //Textura da Comoda
    int conf_texture[] = { 4, 4, 4, 4, 4, 4 };
    
    //Base da comoda
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-5.5f, 0.2f, -25.5);
    glScalef(1.4, 0.05, 2.5);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Tábua intermediária(entre as gavetas)
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-5.5f, 1.4f, -25.0);
    glScalef(1.4, 0.05, 1.4);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Tábua intermediária 2(entre as gavetas)
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-5.5f, 1.0f, -25.0);
    glScalef(1.4, 0.05, 1.4);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Tábua intermediária 3(entre as gavetas)
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-5.5f, 0.6f, -25.0);
    glScalef(1.4, 0.05, 1.4);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Tábua de cima(última tábua)
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-5.5f, 1.8f, -25.5);
    glScalef(1.4, 0.05, 2.5);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Tábua lateral direita
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-5.5f, 1.0f, -26.69);
    glScalef(1.4, 1.6, 0.05);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Tábua lateral esquerda
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-5.5f, 1.0f, -24.3);
    glScalef(1.4, 1.6, 0.05);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Tábua lateral interna
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-5.5f, 1.0f, -25.69);
    glScalef(1.4, 1.54, 0.05);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Fundo da comoda
    glColor3f(0.556863, 0.137255, 0.419608);
    glPushMatrix();
    glTranslatef(-6.175f, 1.0f, -25.5);
    glScalef(0.05, 1.6, 2.5);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Gaveta I
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-5.5f, 1.6f, -25.0);
    glScalef(1.4, 0.33f, 1.4);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Gaveta II
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-5.5f, 1.2f, -25.0);
    glScalef(1.4, 0.33f, 1.4);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Gaveta III
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-5.5f, 0.8f, -25.0);
    glScalef(1.4, 0.33f, 1.4);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Gaveta IV
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-5.5f, 0.4f, -25.0);
    glScalef(1.4, 0.33f, 1.4);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Porta lateral
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-4.84f, 1.0f, -26.19f);
    glScalef(0.1f, 1.5f, 0.9159f);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Puxador da primeira gaveta
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-4.78, 0.4f, -25.0f);
    glScalef(0.0009f, 0.0009f, 0.0009f);
    glutSolidSphere(40, 40, 40);
    glPopMatrix();

    //Puxador da segunda gaveta
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-4.78, 0.8f, -25.0f);
    glScalef(0.0009f, 0.0009f, 0.0009f);
    glutSolidSphere(40, 40, 40);
    glPopMatrix();

    //Puxador da terceira gaveta
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-4.78, 1.2f, -25.0f);
    glScalef(0.0009f, 0.0009f, 0.0009f);
    glutSolidSphere(40, 40, 40);
    glPopMatrix();

    //Puxador da quarta gaveta
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-4.78, 1.6f, -25.0f);
    glScalef(0.0009f, 0.0009f, 0.0009f);
    glutSolidSphere(40, 40, 40);
    glPopMatrix();

    //Puxador da porta II
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-4.78, 1.0f, -25.85f);
    glScalef(0.05f, 0.45f, 0.1f);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Pé da Comoda frontal direito
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-4.88, 0.1f, -26.65f);
    glScalef(0.1f, 0.25f, 0.1f);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Pé da Comoda de trás direito
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-6.05, 0.1f, -26.65f);
    glScalef(0.1f, 0.25f, 0.1f);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Pé da Comoda frontal esquerdo
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-4.88, 0.1f, -24.35f);
    glScalef(0.1f, 0.25f, 0.1f);
    drawCube(1.0f, conf_texture);
    glPopMatrix();

    //Pé da Comoda de trás direito
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-6.05, 0.1f, -24.35f);
    glScalef(0.1f, 0.25f, 0.1f);
    drawCube(1.0f, conf_texture);
    glPopMatrix();
}

void drawShelf()
{
    //Textura das prateleiras
    int shelf_texture[] = { 15, 15, 15, 15, 15, 15 };

    //Prateleira 1
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(0.4f, 3.0f, -31.4);
    glScalef(1.5, 0.2, 0.8);
    drawCube(1.0f, shelf_texture);
    glPopMatrix();

    //Prateleira 2
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(-1.0f, 2.4f, -31.4);
    glScalef(1.5, 0.2, 0.8);
    drawCube(1.0f, shelf_texture);
    glPopMatrix();

    //Prateleira 3
    glColor3f(0.745, 0.423, 0.188);
    glPushMatrix();
    glTranslatef(1.5f, 2.2f, -31.4);
    glScalef(1.5, 0.2, 0.8);
    drawCube(1.0f, shelf_texture);
    glPopMatrix();
}

void drawBeed()
{
    //Textura da cama
    int bed_texture[] = { 4, 4, 4, 4, 4, 4 };

    //Colchao
    glColor3f(255, 255, 255);
    glPushMatrix();
    glTranslatef(4.0f, 0.6f, -11.5);
    glScalef(4.0, 0.4, 2.0);
    drawCube(1.0f, bed_texture);
    glPopMatrix();

    //Mastro
    glColor3f(0.11f, 0.09f, 0.043f);
    glPushMatrix();
    glTranslatef(6.0f, 0.99f, -11.5075);
    glScalef(0.2, 0.6, 2.0);
    drawCube(1.0f, bed_texture);
    glPopMatrix();

    //Perna cabeceira direita
    glColor3f(0.11f, 0.09f, 0.043f);
    glPushMatrix();
    glTranslatef(6.03f, 0.3f, -10.5715);
    glScalef(0.15, 0.785, 0.15);
    drawCube(1.0f, bed_texture);
    glPopMatrix();

    //Perna cabeceira esquerda
    glColor3f(0.11f, 0.09f, 0.043f);
    glPushMatrix();
    glTranslatef(6.0f, 0.3f, -12.434);
    glScalef(0.15, 0.785, 0.15);
    drawCube(1.0f, bed_texture);
    glPopMatrix();

    //Perna de baixo direita
    glColor3f(0.11f, 0.09f, 0.043f);
    glPushMatrix();
    glTranslatef(2.1f, 0.3f, -10.576);
    glScalef(0.15, 0.6, 0.15);
    drawCube(1.0f, bed_texture);
    glPopMatrix();

    //Perna de baixo esquerda
    glColor3f(0.11f, 0.09f, 0.043f);
    glPushMatrix();
    glTranslatef(2.1f, 0.3f, -12.420);
    glScalef(0.15, 0.6, 0.15);
    drawCube(1.0f, bed_texture);
    glPopMatrix();
}

void drawDesk(float x, float y, float z)
{
    //Textura da mesa
    int office_desk_texture[] = { 9, 9, 9, 9, 9, 9 };

    //Cor da mesa
    glColor3f(0.11f, 0.09f, 0.043f);

    //Lateral esquerda
    glPushMatrix();
    glTranslatef(x - 0.20, 0.4f + y, z);
    glScalef(0.04, 1.0f, 1.5);
    drawCube(1.0f, office_desk_texture);
    glPopMatrix();

    //Lateral direita
    glPushMatrix();
    glTranslatef(x + 1.7, 0.4f + y, z);
    glScalef(0.04, 1.0f, 1.5);
    drawCube(1.0f, office_desk_texture);
    glPopMatrix();

    //Parte superior da mesa
    glPushMatrix();
    glTranslatef(x + 0.76, 0.9f + y, z - 0.0);
    glScalef(2.0, 0.08, 1.54);
    drawCube(1.0f, office_desk_texture);
    glPopMatrix();

    //Parte de trás da mesa
    glPushMatrix();
    glTranslatef(x + 0.74f, 0.64f, z - 0.2);
    glScalef(1.9, 0.80f, 0.04);
    drawCube(1.0f, office_desk_texture);
    glPopMatrix();
}

void drawChair(float x, float z)
{
    //Textura da cadeira
    int chair_texture[] = { 2, 2, 2, 2, 2, 2 };

    //Cor da cadeira
    glColor3f(0.0f, 0.0f, 0.0f);

    //suporte esquerdo do apoio traseiro
    glPushMatrix();
    glTranslatef(x + 0.675, 0.5f, z + 0.45);
    glScalef(0.1, 1.20f, 0.1);
    drawCube(1.0f, chair_texture);
    glPopMatrix();

    //segundo suporte esquerdo do apoio traseiro
    glPushMatrix();
    glTranslatef(x + 0.675, 0.25f, z - 0.225);
    glScalef(0.1, 0.75f, 0.1);
    drawCube(1.0f, chair_texture);
    glPopMatrix();

    //assento 
    glPushMatrix();
    glTranslatef(x + 1, 0.6f, z + 0.1);
    glScalef(0.75, 0.1, 0.75);
    drawCube(1.0f, chair_texture);
    glPopMatrix();

    //suporte direito do apoio traseiro
    glPushMatrix();
    glTranslatef(x + 1.325, 0.5f, z + 0.45);
    glScalef(0.1, 1.20f, 0.1);
    drawCube(1.0f, chair_texture);
    glPopMatrix();

    //segundo suporte direito do apoio traseiro
    glPushMatrix();
    glTranslatef(x + 1.325, 0.25f, z - 0.225);
    glScalef(0.1, 0.75f, 0.1);
    drawCube(1.0f, chair_texture);
    glPopMatrix();

    //suporte das costas
    glPushMatrix();
    glTranslatef(x + 1.0f, 1.125f, z + 0.45);
    glScalef(0.75, 0.75f, 0.1);
    drawCube(1.0f, chair_texture);
    glPopMatrix();
}

void drawWindow()
{
    //Desenha a borda esquerda da janela(vendo de dentro)
    glPushMatrix();
    glTranslatef(7.25f, 2.28f, -25.55f);
    glScalef(1.4, 4.4, 0.6);
    glColor3f(1.0, 0.25, 0.0);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha a borda direita da janela(vendo de dentro)
    glPushMatrix();
    glTranslatef(7.25f, 2.28f, -20.45f);
    glScalef(1.4, 4.4, 0.6);
    glColor3f(1.0, 0.25, 0.0);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha a borda inferior
    glPushMatrix();
    glTranslatef(7.25f, 1.17f, -23.0f);
    glScalef(1.4, 1.0, 10.8);
    glColor3f(1.0, 0.25, 0.0);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha a borda superior
    glPushMatrix();
    glTranslatef(7.25f, 3.38f, -23.0f);
    glScalef(1.4, 0.6, 10.8);
    glColor3f(1.0, 0.25, 0.0);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha a borda central
    glPushMatrix();
    glTranslatef(7.25f, 2.3f, -23.0f);
    glScalef(1.4, 4.0, 0.4);
    glColor3f(1.0, 0.25, 0.0);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha janela esquerda
    glPushMatrix();
    glTranslatef(7.25f, 2.325f, -24.19f);
    if(flag_window == 1)
    {
        glRotatef(wind_angle, 0, -40, 0);
        glTranslatef(-1.2, 0.0f, -1.84f);
    }
    glScalef(1.35, 3.68, 4.8);
    glColor3f(0.11f, 0.09f, 0.043f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Parte de vidro da janela esquerda I(superior)
    glPushMatrix();
    glTranslatef(7.25f, 2.75f, -24.75f);
    if (flag_window == 1)
    {
        glRotatef(wind_angle, 0, -40, 0);
        glTranslatef(-0.725, 0.0f, -2.25f);
    }
    glScalef(1.5, 1.5, 1.6);
    glColor4f(0.6f, 0.8f, 0.4f, 0.5f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Parte de vidro da janela esquerda II(superior)
    glPushMatrix();
    glTranslatef(7.25f, 2.75f, -23.75f);
    if (flag_window == 1)
    {
        glRotatef(wind_angle, 0, -40, 0);
        glTranslatef(-1.6, 0.0f, -1.84f);
    }
    glScalef(1.5, 1.5, 1.6);
    glColor4f(0.6f, 0.8f, 0.4f, 0.5f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Parte de vidro da janela esquerda III(inferior)
    glPushMatrix();
    glTranslatef(7.25f, 1.9f, -24.75f);
    if (flag_window == 1)
    {
        glRotatef(wind_angle, 0, -40, 0);
        glTranslatef(-0.725, 0.0f, -2.25f);
    }
    glScalef(1.5, 1.5, 1.6);
    glColor4f(0.6f, 0.8f, 0.4f, 0.5f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Parte de vidro da janela esquerda IV(inferior)
    glPushMatrix();
    glTranslatef(7.25f, 1.9f, -23.75f);
    if (flag_window == 1)
    {
        glRotatef(wind_angle, 0, -40, 0);
        glTranslatef(-1.6, 0.0f, -1.84f);
    }
    glScalef(1.5, 1.5, 1.6);
    glColor4f(0.6f, 0.8f, 0.4f, 0.5f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha janela direita
    glPushMatrix();
    glTranslatef(7.25f, 2.325f, -21.80f);
    if(flag_window == 1)
    {
        glRotatef(120.0f, 0, -40, 0);
        glTranslatef(1.2, 0.0f, -1.84f);
    }
    glScalef(1.35, 3.68, 4.8);
    glColor3f(0.11f, 0.09f, 0.043f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Parte de vidro da janela direita I(superior)
    glPushMatrix();
    glTranslatef(7.25f, 2.75f, -22.26f);
    if (flag_window == 1)
    {
        glRotatef(120.0f, 0, -40, 0);
        glTranslatef(1.6, 0.0f, -1.84f);
    }
    glScalef(1.5, 1.5, 1.6);
    glColor4f(0.6f, 0.8f, 0.4f, 0.5f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Parte de vidro da janela direita II(superior)
    glPushMatrix();
    glTranslatef(7.25f, 2.75f, -21.26f);
    if (flag_window == 1)
    {
        glRotatef(120.0f, 0, -40, 0);
        glTranslatef(0.725f, 0.0f, -2.24f);
    }
    glScalef(1.5, 1.5, 1.6);
    glColor4f(0.6f, 0.8f, 0.4f, 0.5f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Parte de vidro da janela direita III(inferior)
    glPushMatrix();
    glTranslatef(7.25f, 1.9f, -22.26f);
    if (flag_window == 1)
    {
        glRotatef(120.0f, 0, -40, 0);
        glTranslatef(1.6, 0.0f, -1.84f);
    }
    glScalef(1.5, 1.5, 1.6);
    glColor4f(0.6f, 0.8f, 0.4f, 0.5f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Parte de vidro da janela direita IV(inferior)
    glPushMatrix();
    glTranslatef(7.25f, 1.9f, -21.26f);
    if (flag_window == 1)
    {
        glRotatef(120.0f, 0, -40, 0);
        glTranslatef(0.725f, 0.0f, -2.24f);
    }
    glScalef(1.5, 1.5, 1.6);
    glColor4f(0.6f, 0.8f, 0.4f, 0.5f);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha maçaneta esquerda inferior
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(7.55f, 1.80f, -25.5f);
    glScalef(0.004f, 0.04f, 0.004f);
    glutSolidCylinder(40.0f, 15.0f, 100);
    glPopMatrix();

    //Desenha maçaneta esquerda superior
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(7.55f, 2.80f, -25.5f);
    glScalef(0.004f, 0.04f, 0.004f);
    glutSolidCylinder(40.0f, 15.0f, 100);
    glPopMatrix();

    //Desenha maçaneta direita inferior
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(7.55f, 1.80f, -20.5f);
    glScalef(0.004f, 0.04f, 0.004f);
    glutSolidCylinder(40.0f, 15.0f, 100);
    glPopMatrix();

    //Desenha maçaneta direita superior
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(7.55f, 2.80f, -20.5f);
    glScalef(0.004f, 0.04f, 0.004f);
    glutSolidCylinder(40.0f, 15.0f, 100);
    glPopMatrix();

}

void drawDoor()
{
    //Porta
    glPushMatrix();
    glColor3f(0.73, 0.16, 0.96);
    glTranslatef(-0.75f, 1.5f, -9.4f);
    if(flag_door == 1)
    {
        glRotatef(door_angle, 0, -400, 0);
        glTranslatef(-1.4, 0.0f, -0.92f);
    }
    glScalef(2.0, 3.04, 0.4f);
    glutSolidCube(1.0);
    glTranslatef(-0.75f, 1.5f, -9.4f);
    glPopMatrix();

    //Trinco
    glColor3f(0.6, 0.8, 0.196078);
    glPushMatrix();
    glTranslatef(-1.5f, 1.50f, -9.15f);
    if(flag_door == 1)
    {
        glRotatef(door_angle, 0, -400, 0);
        glTranslatef(-2.65, 0.0f, -1.2f);
    }
    glScalef(0.003f, 0.003f, 0.003f);
    glutSolidSphere(40, 40, 40);
    glPopMatrix();

    //Desenha borda superior da porta
    glPushMatrix();
    glTranslatef(-0.75f, 3.10f, -9.25f);
    glScalef(4.5, 0.3, 1.4);
    glColor3f(0.6, 0.8, 0.196078);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha borda esquerda da porta
    glPushMatrix();
    glTranslatef(-1.8f, 1.575f, -9.25f);
    glScalef(0.3, 6.25, 1.4);
    glColor3f(0.6, 0.8, 0.196078);
    glutSolidCube(0.5);
    glPopMatrix();

    //Desenha borda direita da porta
    glPushMatrix();
    glTranslatef(0.3f, 1.575f, -9.25f);
    glScalef(0.3, 6.25, 1.4);
    glColor3f(0.6, 0.8, 0.196078);
    glutSolidCube(0.5);
    glPopMatrix();
}

void drawRoom()
{
    //Textura das paredes
    int p_texture[] = { 1, 1, 1, 1, 1, 1 };

    //Parede lateral esquerda
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(-7.25f, 2.75f, -20.5f);
    glScalef(0.5, 5.5, 23.0);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Parede lateral direita I
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(7.25f, 2.75f, -14.75f);
    glScalef(0.5, 5.5, 11.5);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Parede lateral direita II
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(7.25f, 2.75f, -28.75f);
    glScalef(0.5, 5.5, 6.5);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Parede lateral direita inferior 
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(7.25f, 0.7f, -23.00f);
    glScalef(0.5, 1.4, 5.5);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Parede lateral direita superior
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(7.25f, 4.5f, -23.00f);
    glScalef(0.5, 2.5, 5.5);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Parede do fundo do quarto 
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(0.0f, 2.75f, -31.75f);
    glScalef(14.5, 5.5, 0.5);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Parede da frente esquerda
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(-4.5f, 2.75f, -9.25f);
    glScalef(5.4, 5.5, 0.5);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Parede da frente direita
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(3.75f, 2.75f, -9.25f);
    glScalef(6.75, 5.5, 0.5);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Parede superior da entrada
    glPushMatrix();
    glColor3f(0.85f, 0.95f, 0.85f);
    glTranslatef(0.0f, 4.55f, -9.25f);
    glScalef(14.5, 2.75, 0.5);
    drawCube(1.0f, p_texture);
    glPopMatrix();

    //Teto do quarto
    glPushMatrix();
    glColor3f(0.66f, 0.66f, 0.66f);
    glTranslatef(0.0f, 5.75f, -20.5f);
    glScalef(15.0, 0.5, 23.0);
    drawCube(1.0f, p_texture);
    glPopMatrix();
}

void drawFloor()
{
    //Piso do quarto
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture_id[7]);
    glTranslatef(0.0f, 0.0f, -13.0f);
    glBegin(GL_QUADS);
    glColor3f(0.66f, 0.66f, 0.66f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-7.5f, 0.0f, -19.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-7.5f, 0.0f, 4.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(7.5f, 0.0f, 4.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(7.5f, 0.0f, -19.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void display()
{
    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // To operate on model-view matrix
    glMatrixMode(GL_MODELVIEW);
    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(x, y, z,
        x + lx, y, z + lz,
        0.0f, 1.0f, 0.0f);

    drawChair(5.0f, -29.0f);
    drawFloor();
    drawRoom();
    drawDesk(5.0f, 0.1f, -30.0f);
    drawBeed();
    drawShelf();
    drawComfortable();
    drawWritingBoard();
    drawPictr();
    drawPoleDance();
    drawDoor();
    drawWindow();
    drawDecorations();
    draw_lightfix();

    //Desenha o quadro do Van Gogh
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-4.75f, 2.75f, -31.4);
    aply_texture(0);
    glScalef(1.25, 1.125, 0.125);
    do_texture();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    drawAeretor();

    glFlush();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'o':
        if(door_angle < 120.0f)
        {
            door_angle = 120.0f;
            flag_door = 1;
            glutPostRedisplay();
            break;
        }

    case 'c':
        if(door_angle >= 120.0f)
        {
            door_angle = 0;
            flag_door = 0;
            glutPostRedisplay();
            break;
        }

    case 'w':
        if(wind_angle < 60.0f)
        {
            wind_angle = 60.0f;
            flag_window = 1;
            glutPostRedisplay();
            break;
        }
        glutPostRedisplay();
        break;

    case 't':
        if(wind_angle >= 60.0f)
        {
            wind_angle = 0;
            flag_window = 0;
            glutPostRedisplay();
            break;
        }
        glutPostRedisplay();
        break;
    }
}

void processSpecialKeys(int key, int xx, int yy)
{

    float fraction = 0.5f;
    float fraction_2 = 0.1f;

    switch (key)
    {
    case GLUT_KEY_LEFT:
        angle -= 0.05f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_RIGHT:
        angle += 0.05f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_UP:
        x += lx * fraction;
        z += lz * fraction;
        break;
    case GLUT_KEY_DOWN:
        x -= lx * fraction;
        z -= lz * fraction;
        break;
    case GLUT_KEY_PAGE_UP:
        y += y * fraction_2;
        break;
    case GLUT_KEY_PAGE_DOWN:
        y -= y * fraction_2;
        break;
    }
}

void reshape(GLsizei width, GLsizei height)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (height == 0)
    {
        height = 1;
    }

    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    float ratio = width * 1.0 / height;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);

}

void setup_lighting()
{
    //Iluminacao
    glEnable(GL_LIGHTING); // Habilita a iluminacao
    glEnable(GL_LIGHT0); // Habilita a luz 0
    glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade
    glEnable(GL_COLOR_MATERIAL);// Habilita cor dos materiais


    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    
    float mat_ambient[] = {0.2f , 0.2f ,0.2f , 1.0f};
    float mat_diffuse[] = {1.0f , 0.0f, 0.0f , 1.0f}; // Cor azul
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Destaquesbrancos
    float mat_shininess[] = {50.0f}; // O quao polida eh a superficie

    float spot_direction[] = {0.0f, -1.0f, 0.0f};
    float spot_cutoff[] = {90.0f};

    glLightfv(GL_MAX_LIGHTS-1, GL_SPOT_DIRECTION, spot_direction);
    glLightfv(GL_MAX_LIGHTS-1, GL_SPOT_CUTOFF, spot_cutoff);

     //O primeiro parametro diz qual lado da face ( GL_FRONT ,GL_BACK etc )
     glLightfv(GL_FRONT, GL_AMBIENT, mat_ambient);
     glLightfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
     glLightfv(GL_FRONT, GL_SPECULAR, mat_specular);
     glLightfv(GL_FRONT, GL_SHININESS, mat_shininess);

     // Luz spot
     glLightfv(GL_LIGHT4, GL_POSITION, posLuzSpot);
     glLightfv(GL_LIGHT4, GL_DIFFUSE, luzDifusaSpot);
     glLightfv(GL_LIGHT4, GL_SPECULAR, luzEspecSpot);
     glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dirLuzSpot);
     glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 90);
     glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10);
}


void init()
{
    //glClearColor(0.7, 1.0, 1.0, 0.0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
}

int main(int argc, char** argv)
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);

    if (!glutCreateWindow("David's Room"))
    {
        fprintf(stderr, "Error opening a window.\n");
        exit(-1);
    }

    setup_lighting();
    init_obj_vecs();
    init_textures_vec();

    init();

    glutKeyboardFunc(keyboard);
    //setup_lighting();

   load_texture("vangogh.jpg", 0);
   //load_texture("ceramica.jpg", 1);


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutSpecialFunc(processSpecialKeys);

    glutMainLoop();

    return 0;
}