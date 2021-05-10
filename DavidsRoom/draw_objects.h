/*
* Esse arquivo serve SOMENTE como apoio pro main.c
*/
#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H

typedef struct TextureInfo TextureInfo;

/* Guarda todos os ids das texturas carregadas*/
extern TextureInfo** textures;

/*Desenha um objeto qualquer*/
void draw_objects(int index, float r, float g, float b, int ap_texture);

/*Desenha um cubo*/
void do_texture();

/*
Carrega uma imagem localizada em path para uma textura e associa seu id na posi��o index
Se o arquivo n�o puder ser carregado o programa � encerrado
*/
void load_texture(const char* path, int index);

/*
Inicializa o vetor de ID's de texturas
*/
void init_textures_vec();

/*
Aplica uma textura com seu id na posi��o index do vetor textures
*/
void aply_texture(int index);
#endif