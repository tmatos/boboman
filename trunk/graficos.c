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

#include <allegro.h>
#include "msg.h"
#include "graficos.h"
#include "jogo.h"

TipoBloco carrega_blocos() {

	PALLETE pal;
	TipoBloco bloco;
	
	bloco.pedra = load_bitmap("img/pedra.png",pal);
	if( !bloco.pedra ) erro(falta_arquivo, "img/pedra.png");
	bloco.chao = load_bitmap("img/chao.png",pal);
	if( !bloco.chao ) erro(falta_arquivo, "img/chao.png");
	bloco.bloco = load_bitmap("img/bloco.png",pal);
	if( !bloco.bloco ) erro(falta_arquivo, "img/bloco.png");
	bloco.espinho = load_bitmap("img/espinho.png",pal);
	if( !bloco.espinho ) erro(falta_arquivo, "img/espinho.png");
	bloco.pilha = load_bitmap("img/pilha.png",pal);
	if( !bloco.pilha ) erro(falta_arquivo, "img/pilha.png");
	bloco.mola = load_bitmap("img/mola.png",pal);
	if( !bloco.mola ) erro(falta_arquivo, "img/mola.png");
	bloco.monstro = load_bitmap("img/monstro.png",pal);
	if( !bloco.monstro ) erro(falta_arquivo, "img/monstro.png");
	
	return(bloco);
}
END_OF_FUNCTION(carrega_blocos);

// desenha o fundo
// TODO: fazer esta funcao de verdade
void desenha_fundo( TipoCenario *cenario, BITMAP *fundo, BITMAP *fundo2 ) {
	set_alpha_blender();
	//--cenario obsoleto-----------------------------------------------
	clear_to_color(fundo2, makecol(70,255,255));
	draw_trans_sprite(fundo2, cenario->nuvem, HRES-350-cenario->posicao, VRES-430);
	clear_to_color(fundo, makecol(255,0,255));
	draw_trans_sprite(fundo, cenario->coco, HRES-200-cenario->posicao, VRES-370);
	draw_trans_sprite(fundo, cenario->sol, 40-cenario->posicao, 20);
	//-----------------------------------------------------------------
}
END_OF_FUNCTION(desenha_fundo);

// soh muda o shape do cara (no momento talvez)
void move_cara( TipoCara *cara, TipoDirecao dir ) {
	if( dir==direita) {
		if( cara->img == cara->img_frente ) cara->img = cara->img_frente_anda;
		else if( cara->img == cara->img_frente_anda ) cara->img = cara->img_frente;
		else if( cara->img == cara->img_virado ) cara->img = cara->img_frente;
		else if( cara->img == cara->img_virado_anda ) cara->img = cara->img_frente;	
	}
	else if( dir==esquerda) {
		if( cara->img == cara->img_frente ) cara->img = cara->img_virado;
		else if( cara->img == cara->img_frente_anda ) cara->img = cara->img_virado;
		else if( cara->img == cara->img_virado ) cara->img = cara->img_virado_anda;
		else if( cara->img == cara->img_virado_anda ) cara->img = cara->img_virado;
	}
}
END_OF_FUNCTION(move_cara);
