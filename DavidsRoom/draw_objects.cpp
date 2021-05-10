#include <stdio.h>
#include <GL/glut.h>
#include "draw_objects.h"
#include "obj_import.h"
#include "stb_image.h"

struct TextureInfo {
    unsigned int textureID;
};


int WINDOW_OPEN = 0;//flag para abrir a janela
int HEX_ANGLE = 0;

int DOOR_OPEN = 0;
int DOR_ROT = 0;

float OFF_SET_COL = 0.1;

TextureInfo** textures = NULL;

void do_texture()
{

    glBegin(GL_QUADS);
    //glColor3f (0.5 , 0.5 , 0.5 ) ;
    glColor3f(1, 1, 1);


    glTexCoord2f(0, 0);
    glNormal3f(0, 0, 1);
    glVertex3f(0, 0, 0);



    glTexCoord2f(1, 0);
    glNormal3f(0, 0, 1);
    glVertex3f(1.5, 0, 0);



    glTexCoord2f(1, 1);
    glNormal3f(0, 0, 1);
    glVertex3f(1.5, 1, 0);


    glTexCoord2f(0, 1);
    glNormal3f(0, 0, 1);
    glVertex3f(0, 1, 0);


    glEnd();

}

void draw_objects(int index, float r, float g, float b, int ap_texture)
{
    if (0 <= index < MODEL_QUANT)
    {
        int i;

        VERTICES = vecs[index]->VERTICES;
        NORMALS = vecs[index]->NORMALS;
        TEX_COORDS = vecs[index]->TEX_COORDS;
        VERTEX_COUNT = vecs[index]->VERTEX_COUNT;

        if (ap_texture)
        {
            OFF_SET_COL = 0;
        }
        else
        {
            OFF_SET_COL = 0.1;
        }

        glBegin(GL_TRIANGLES);
        for (i = 0; i < VERTEX_COUNT; i++)
        {
            glColor3f(r + OFF_SET_COL, g + OFF_SET_COL, b + OFF_SET_COL);
            glTexCoord2f(TEX_COORDS[i].x, TEX_COORDS[i].y);
            glNormal3f(NORMALS[i].x, NORMALS[i].y, NORMALS[i].z);
            glVertex3f(VERTICES[i].x, VERTICES[i].y, VERTICES[i].z);
            OFF_SET_COL = -OFF_SET_COL;
        }
        glEnd();

    }
    else
    {
        printf("Indice fora do range permitido: [0, %d)", MODEL_QUANT);
        exit(1);
    }
}

void load_texture(const char* path, int index)
{
    //printf("Valor do index e do Model_QUANT: %d %d\n", index, MODEL_QUANT);
    if (0 <= index <= MODEL_QUANT)
    {
        //printf("Entrei aqui\n");
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        //printf("Entrei aqui ¬¬\n");
        //printf("%c", data);
        if(data)
        {
            printf("Entrei aqui\n");
            textures[index] = (TextureInfo*)malloc(sizeof(TextureInfo));

            unsigned int textureID;
            glGenTextures(1, &textureID);

            textures[index]->textureID = textureID;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        }
        else
        {
            printf("Ta dando erro aqui lkkk\n");
            printf("Falha ao carregar a textura.");
            stbi_image_free(data);
            exit(1);
        }
        stbi_image_free(data);

    }
    else
    {
        printf("Indice de textura inválido.");
        exit(1);
    }

}

void init_textures_vec()
{
    textures = (TextureInfo**)malloc(MODEL_QUANT * sizeof(TextureInfo*));

    int i;
    for (i = 0; i < MODEL_QUANT; i++)
    {
        textures[i] = NULL;
    }
}

void aply_texture(int index)
{
    if (0 <= index <= MODEL_QUANT)
    {
        if (textures[index])
        {
            unsigned int textureID = textures[index]->textureID;
            glBindTexture(GL_TEXTURE_2D, textureID);
        }
    }
    else
    {
        printf("Indice de textura inválido.");
        exit(1);
    }
}