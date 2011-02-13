/*
    Boboman 0.1
    Copyright (C) 2008,2011  Tiago Matos

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// Rotinas p/ controle do jogo

#ifndef JOGO_H

#define JOGO_H

#define MAXLIN 15 // tamanho maximo de linhas da matriz de objetos
#define MAXCOL 600 // tamanho maximo de colunas da matriz de objetos
#define HRES 640
#define VRES 480
#define NUM_VIDAS 3
#define VELOCIDADE_DO_PULO 6
#define VELOCIDADE_NORMAL 2
#define VELOCIDADE_CORRENDO 3
#define FREQ_SHAPE 8 // frequencia de mudanca do shape do cara ao andar
#define BLOCKSIZE 32 // lado em pixels de um bloquinho quadrado

typedef struct estrutura_auto {
	int velocidade, x, y;
	TipoDirecao direcao;
} TipoAuto;

typedef struct estrutura_objeto {
	BITMAP *figura;
	int solido, letal, final, item;
	TipoAuto *bicho;
} TipoObjeto;

typedef struct estrutura_fase {
	int numero;
	char *arquivo; // nome do arquivo da fase
	struct estrutura_fase *proxima; // apontador p/ proxima fase, isso eh uma lista :P
} TipoFase;

void abertura();
void reseta( TipoCara *cara, TipoCenario *cenario, BITMAP *fundo, BITMAP *fundo2, int *tempo );
void carrega_fase(char* nome, TipoObjeto matrix[MAXLIN][MAXCOL], TipoCenario *cenario, TipoBloco bloco);
int colisao( TipoDirecao direcao, TipoCara *cara, TipoCenario *cenario, TipoObjeto matrix[MAXLIN][MAXCOL] );
void apaga( TipoObjeto *obj );
void carrega_jogo( char *nome_arq_jogo, TipoFase *lista_fase );
void inicializa( TipoObjeto matrix[MAXLIN][MAXCOL] );
void move_objeto( int l, int c, TipoObjeto matrix[MAXLIN][MAXCOL], int lb, int cb );
void carrega_config(int *TELA_CHEIA);

#endif
