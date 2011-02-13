// joguinho bobo
// v. 0005
// por Tiago o Mestre da Armengologia Universal Ilimitada
//
// Rotinas p/ manipulacao dos graficos

#ifndef GRAFICOS_H

#define GRAFICOS_H

typedef enum { esquerda, direita, abaixo, acima } TipoDirecao;

// estrutura para os shapes dos bloquinhos de 32x32
typedef struct {
	BITMAP *pedra;
	BITMAP *chao;
	BITMAP *bloco;
	BITMAP *espinho;
	BITMAP *pilha;
	BITMAP *mola;
	BITMAP *monstro;
} TipoBloco;

typedef struct estrutura_cenario {
	BITMAP *coco;
	BITMAP *sol;
	BITMAP *nuvem;
	int posicao, tamanho, tempo_max;
} TipoCenario;

typedef struct estrutura_cara {
	BITMAP *img_frente;
	BITMAP *img_virado;
	BITMAP *img_frente_anda;
	BITMAP *img_virado_anda;
	int posicao, altura, velocidade_ver, velocidade_hor, tempo, up_off, res_x, res_y;
	BITMAP *img;
	int vidas, morte, passou, energia, velocidade_pulo;
} TipoCara;

TipoBloco carrega_blocos();
void desenha_fundo( TipoCenario *cenario, BITMAP *fundo, BITMAP *fundo2 );
void move_cara( TipoCara *cara, TipoDirecao dir );

#endif
