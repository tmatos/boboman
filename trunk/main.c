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

#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <alpng.h>
#include "lang.h"
#include "msg.h"
#include "menus.h"
#include "graficos.h"
#include "jogo.h"
#include "main.h"

volatile long speed_counter = 0;

int TELA_CHEIA = 1;

int main(int argc, char *argv[]) {

	init();
	set_alpha_blender();

	BITMAP *bufa = create_bitmap(HRES,VRES);
	clear(bufa);
	
	BITMAP *fundo = create_bitmap(HRES,VRES);
	clear(fundo);

	BITMAP *fundo2 = create_bitmap(HRES,VRES);
	clear(fundo2);

	PALETTE pal;

	TipoFase lista_fase;
	carrega_jogo("jogo.cfg", &lista_fase);

	TipoCara cara;
	TipoCenario cenario;
	
	int tempo; // contador de tempo global no jogo
	int frames = 0; // frames desenhados no segundo corrente
	time_t segundos = time(NULL); // contador de segundos
	int fps = 0; // taxa de frames por segundo
	int pular_frame = 0;
	TipoFase *fase; // apontador para a fase atual
	fase = &lista_fase; // vamos p/ a primeira fase da lista
	
	TipoBloco bloco = carrega_blocos();

	TipoObjeto matrix[MAXLIN][MAXCOL]; // a matriz de sprites

	int i,j; // usados no loop principal para fins diversos
	
	inicializa(matrix);

	carrega_fase(fase->arquivo, matrix, &cenario, bloco);
	reseta( &cara, &cenario, fundo, fundo2, &tempo );

	// o que vemos comeca aqui
	abertura();
	menu(&speed_counter);
	
//	if(SOM) {
		MIDI *musica = load_midi("musica/primeira.mid");
		if( !musica ) erro( falta_arquivo, "musica/primeira.mid" );
		MIDI *musica_fim = load_midi("musica/fim.mid");
		if( !musica_fim ) erro( falta_arquivo, "musica/fim.mid" );
		play_midi(musica, TRUE);
//		SAMPLE *passou = load_sample("som/passou.wav");
//		if( !passou ) erro( falta_arquivo, "som/passou.wav" );
//	}

	speed_counter=1; // correcao da velocidade
	
	for(;;) {
		while(speed_counter > 0){

			if( key[KEY_UP] && cara.up_off && colisao(abaixo,&cara,&cenario,matrix) ) {
				cara.up_off = 0;
				cara.altura--; // ele sobe
				cara.velocidade_ver = cara.velocidade_pulo;
				cara.tempo = 1;
			}
			if(!key[KEY_UP] ) cara.up_off=1; // up_off serve p/ o cara nao voar se apertar muito up

			if( key[KEY_LSHIFT] ) cara.velocidade_hor = VELOCIDADE_CORRENDO; // tah meio estranho, ve como eh mario e dah um jeito
			else cara.velocidade_hor = VELOCIDADE_NORMAL;

			if( key[KEY_LEFT] && !colisao(esquerda, &cara, &cenario, matrix) ) {
				if( (cara.posicao > HRES/2-10) || ((cenario.posicao==0) && (cara.posicao > 5)) ) {
					if( tempo%FREQ_SHAPE == 0 ) move_cara( &cara, esquerda ); // tem um problema se usar isso assim, no jogo ao aperta esq ou dir mto rapido o cara anda de costas
					cara.posicao -= cara.velocidade_hor;
				}
				else if( cenario.posicao > 0 ) {
					if( (cenario.posicao - cara.velocidade_hor) < 0 ) cenario.posicao=0;
					else cenario.posicao -= cara.velocidade_hor;
					if( tempo%FREQ_SHAPE == 0 ) move_cara( &cara, esquerda );
					desenha_fundo( &cenario, fundo, fundo2 );
				}
				if( colisao(esquerda, &cara, &cenario, matrix) ) cara.posicao += cara.velocidade_hor;
			}
			if( key[KEY_RIGHT] && !colisao(direita, &cara, &cenario, matrix) ) {
				if( (cara.posicao < HRES/2+10) || ( (cenario.posicao == cenario.tamanho-20*BLOCKSIZE) && (cara.posicao < (HRES - cara.res_x - 5)) ) ) {
					if( tempo%FREQ_SHAPE == 0 ) move_cara( &cara, direita );
					cara.posicao += cara.velocidade_hor;
				}
				else if( cenario.posicao < (cenario.tamanho - 20*BLOCKSIZE) ) {
					if( (cenario.posicao + cara.velocidade_hor) > (cenario.tamanho - 20*BLOCKSIZE) ) cenario.posicao = cenario.tamanho - 20*BLOCKSIZE;
					else cenario.posicao += cara.velocidade_hor;
					if( tempo%FREQ_SHAPE == 0 ) move_cara( &cara, direita );
					desenha_fundo( &cenario, fundo, fundo2 );
				}
				if(colisao(direita, &cara, &cenario, matrix)) cara.posicao -= cara.velocidade_hor;
			}
			
			// gastar energia :)
			if( key[KEY_SPACE] && (cara.energia > 0) && (tempo%4 == 0) ) {
				cara.energia--;
				cara.velocidade_pulo = VELOCIDADE_DO_PULO*2 - 1;
			}
			else if( !key[KEY_SPACE] || (cara.energia == 0) ) cara.velocidade_pulo = VELOCIDADE_DO_PULO;
			
			if(key[KEY_ESC]) {
				TipoEscolha resposta = nao;
				while( !key[KEY_ENTER] ) {
					// caixa de dialogo
					
					rectfill( bufa, HRES/2-120, VRES/2-40, HRES/2+120, VRES/2+40, makecol(255,255,255 ) );
					textout_ex(bufa, font, TEXT_MENU_LEAVE, HRES/2-100, VRES/2-25, makecol(0,0,0), -1);
					if( resposta==sim ) {
						textout_ex(bufa, font, TEXT_YES_UPPER, HRES/2-65, VRES/2+5, makecol(255,0,0), -1);
						textout_ex(bufa, font, TEXT_NO_LOWER, HRES/2+35, VRES/2+5, makecol(0,0,0), -1);
					}
					else {
						textout_ex(bufa, font, TEXT_YES_LOWER, HRES/2-65, VRES/2+5, makecol(0,0,0), -1);
						textout_ex(bufa, font, TEXT_NO_UPPER, HRES/2+35, VRES/2+5, makecol(255,0,0), -1);
					}
					if( key[KEY_LEFT] && resposta==nao ) resposta=sim;
					if( key[KEY_RIGHT] && resposta==sim ) resposta=nao;
					
					blit(bufa, screen, 0, 0, 0, 0, HRES, VRES);
				}
				if( resposta==sim ) { // tem que mudar estrutura do jogo p/ desmontar mais e tirar esse armengue
					fase = &lista_fase;
					inicializa(matrix);
					carrega_fase(fase->arquivo, matrix, &cenario, bloco);
					reseta( &cara, &cenario, fundo, fundo2, &tempo );
                    play_midi(musica, FALSE);					
					menu(&speed_counter);
					play_midi(musica, TRUE);
				}
				speed_counter=1; // mais uma correcao da velocidade
			}
			
			// cabecada no teto
			if( colisao(acima,&cara,&cenario,matrix) ) cara.velocidade_ver = -1;

			// "gravidade"
			if( !colisao(abaixo,&cara,&cenario,matrix) ) {

				cara.altura = cara.altura - cara.velocidade_ver;

				if( colisao(abaixo,&cara,&cenario,matrix) ) {
					cara.altura = cara.altura + cara.velocidade_ver + 1;
				}

				if( cara.tempo==5 ) {
					cara.tempo = 1;
					cara.velocidade_ver--;
				}
				else cara.tempo++;

//				if( cara.velocidade_pulo > VELOCIDADE_DO_PULO ) draw_tran_sprite(bufa, bloco.mola, cara.posicao, cara.altura + cara.res_y + BLOCKSIZE); // nao sei porque isso nao funciona
			}
// isso eh apenas um workarround p/ despistar o bug ao pular e andar enconstado em 2 blocos empilhados
			else cara.velocidade_ver=0;
// outra hora eh p/ corrigir de verdade -------------------------------------------------

			// de segundo em segundo atualiza o valor de fps
			if( segundos != time(NULL) ) {
				segundos = time(NULL);
				fps = frames;
				frames = 0;
			}
			
			// calcula se deve pular frame
//			if( fps < 100 ) pular_frame = 1;
//			else pular_frame = 0;

/////// Parte que desenha os bagulhos na tela //////////////////////////////////
			if( !pular_frame ) {
				frames++;
				
				// limpa o bufer primero
				clear_to_color(bufa,makecol(255,255,255));
	
				// depois desenha os fundos e o cara
				draw_sprite(bufa, fundo2, 0, 0);
				draw_sprite(bufa, fundo, 0, 0);
				draw_trans_sprite(bufa, cara.img, cara.posicao, cara.altura);
	
				// desenha sprites
				for( i=0 ; i<MAXLIN ; i++ ) {
					for( j = cenario.posicao/BLOCKSIZE ; j < cenario.posicao/BLOCKSIZE + 21 ; j++ ) {
//- o movimento de um bicho na tela ----------------------------UNDER-DEVELOPMENT------------
						if( matrix[i][j].bicho ) {
							// matar o cara
							if( matrix[i][j].letal ) {
								if( ((i*BLOCKSIZE > cara.altura) && (i*BLOCKSIZE < cara.altura+cara.res_y)) || ((i*BLOCKSIZE+BLOCKSIZE > cara.altura) && i*BLOCKSIZE+BLOCKSIZE < (cara.altura+cara.res_y)) ) { // implementar depois o offset vertical
									if( ( (cenario.posicao+cara.posicao > j*BLOCKSIZE+matrix[i][j].bicho->x) && (cenario.posicao+cara.posicao < j*BLOCKSIZE+matrix[i][j].bicho->x+BLOCKSIZE) )
									|| ( (cenario.posicao+cara.posicao+cara.res_x > j*BLOCKSIZE+matrix[i][j].bicho->x) && (cenario.posicao+cara.posicao+cara.res_x < j*BLOCKSIZE+matrix[i][j].bicho->x+BLOCKSIZE) ) ) {
										cara.morte = 1;
									}
								}
							}
//							if( matrix[i][j].bicho->direcao == direita ) {
								draw_trans_sprite(bufa, matrix[i][j].figura, /*matrix[i][j].bicho->x*/j*BLOCKSIZE - cenario.posicao + matrix[i][j].bicho->x, matrix[i][j].bicho->y/*i*BLOCKSIZE*/);
//							}
//							else {
//								draw_trans_sprite(bufa, matrix[i][j].figura, /*matrix[i][j].bicho->x*/j*BLOCKSIZE - cenario.posicao - matrix[i][j].bicho->velocidade, matrix[i][j].bicho->y/*i*BLOCKSIZE*/);
//							}
							if( matrix[i][j].bicho->x == 0 && (matrix[i][j+1].solido || matrix[i][j-1].solido) ) {
								if( matrix[i][j].bicho->direcao == direita ) matrix[i][j].bicho->direcao = esquerda;
								else matrix[i][j].bicho->direcao = direita;
							}
//							if( tempo%2 == 0 ) {
								if( matrix[i][j].bicho->direcao == direita ) matrix[i][j].bicho->x++;
								else if( matrix[i][j].bicho->direcao == esquerda ) matrix[i][j].bicho->x--;
//							}
							if( matrix[i][j].bicho->x == BLOCKSIZE+1 ) {
								matrix[i][j].bicho->x = 0;
								move_objeto( i, j, matrix, i, j+1 );
							} else
							if( matrix[i][j].bicho->x == -(BLOCKSIZE+1) ) {
								matrix[i][j].bicho->x = 0;
								move_objeto( i, j, matrix, i, j-1 );
							}
						}
//-------------------------------------------------------------------------------------------
						else if( matrix[i][j].figura ) draw_trans_sprite(bufa, matrix[i][j].figura, j*BLOCKSIZE - cenario.posicao, i*BLOCKSIZE);
					}
				}
	
				// infos - score, tempo, energia, fps
				textprintf_ex(bufa, font, 10, 10, makecol(255, 100, 200), -1, TEXT_TIME, tempo/100);
				textprintf_ex(bufa, font, HRES-80, 10, makecol(255, 100, 200), -1, TEXT_LIFE, cara.vidas);
				textprintf_ex(bufa, font, HRES/2-120, 10, makecol(255, 100, 200), -1, TEXT_ENERGY);
				textprintf_ex(bufa, font, HRES-65, 35, makecol(255, 100, 200), -1, "FPS: %d", fps);
				rectfill( bufa, HRES/2-50, 5, HRES/2-50 + cara.energia, 20, makecol(255,230,20 ) );
				
				blit(bufa, screen, 0, 0, 0, 0, HRES, VRES); // bota o bufer na tela
			}
////// Fim dos desenhos ////////////////////////////////////////////////////////

			// tratamento de mortes ...
			// como time over ou cair no abismo ou o que for ...
			if( cara.morte || tempo<0 || cara.altura > VRES ) {

				if( tempo<0 ) exibir(tempo_acabou);		
				else exibir(opa);
				speed_counter = 0;
				j=0;
				if( cara.vidas ) j = cara.vidas;
				else {
					if(SOM) {
						play_midi(musica, FALSE);
						play_midi(musica_fim, TRUE);
					}
					game_over();
					if(SOM) {
						play_midi(musica_fim, FALSE);
					}
					fase = &lista_fase;
					menu(&speed_counter);
					play_midi(musica, TRUE);
				}
				inicializa(matrix);
				carrega_fase(fase->arquivo, matrix, &cenario, bloco);
				reseta( &cara, &cenario, fundo, fundo2, &tempo );
				if(j) cara.vidas = --j;
			}
			
			// passar de fase ...
			if( cara.passou ) {
				if( fase->proxima->arquivo ) {
					fase = fase->proxima;
					exibir(passou_fase);
					speed_counter = 0;
					j = cara.vidas;
				}
				else {
				    play_midi(musica, FALSE);
//					play_sample( passou, 235, 127, 1000, 0 );
//					game_over();
					exibir(creditos);
					speed_counter = 0;
//					stop_sample(passou);
					menu(&speed_counter);
					play_midi(musica, TRUE);
					fase = &lista_fase;
					j = NUM_VIDAS;
				}
				inicializa(matrix);
				carrega_fase(fase->arquivo, matrix, &cenario, bloco);
				reseta( &cara, &cenario, fundo, fundo2, &tempo );
				cara.vidas = j;
			}
			tempo--;
			speed_counter --;
		}
	}
	deinit();
	return 0;
}
END_OF_MAIN()

void init() {
	carrega_config(&TELA_CHEIA);
	int depth, res;
	allegro_init();
	alpng_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 16;
	set_color_depth(depth);
	if(TELA_CHEIA) res = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, HRES, VRES, 0, 0);
	else res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, HRES, VRES, 0, 0);
	if (res != 0) {
//		allegro_message(allegro_error);
		erro( sem_modo_grafico, "" );
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	if(SOM) install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	/* add other initializations here */
	LOCK_VARIABLE(speed_counter); //Used to set the timer - which regulates the game's
	LOCK_FUNCTION(increment_speed_counter);//speed.
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(100));//Set our BPS

	set_window_title(TEXT_WINDOW_TITLE);
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}

void increment_speed_counter() // A function to increment the speed counter
{
	speed_counter++; // This will just increment the speed counter by one. :)
}
END_OF_FUNCTION(increment_speed_counter);
