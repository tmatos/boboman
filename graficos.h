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
