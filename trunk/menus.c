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

// Rotinas p/ os menus do jogo

#include <allegro.h>
#include "lang.h"
#include "msg.h"
#include "graficos.h"
#include "jogo.h"
#include "main.h"
#include "menus.h"

// o menu principal do jogo
void menu(volatile long *speed_counter)
{
	int enter_on = key[KEY_ENTER], travado = 0;
	enum { jogar, opcao, sair } selecao = jogar;
	MIDI *tema = load_midi("musica/tema.mid");
	if( !tema ) erro( falta_arquivo, "musica/tema.mid" );

	clear_to_color(screen, makecol(230,230,230));
	play_midi(tema, TRUE);

	for(;;) {
		
		textprintf_ex(screen, font, 200, 170, makecol(0,0,0), -1, TEXT_MAIN_NAME, VERSAO);
		textprintf_ex(screen, font, HRES-320, VRES-30, makecol(0,0,0), -1, TEXT_MAIN_COMPILED, __DATE__, __TIME__);
		if( selecao==jogar ) textout_ex(screen, font, TEXT_MAIN_PLAY_UP, 260, 270, makecol(255,0,0), -1);
		else textout_ex(screen, font, TEXT_MAIN_PLAY_LOW, 260, 270, makecol(0,0,0), -1);
		if( selecao==opcao ) textout_ex(screen, font, TEXT_MAIN_OPTION_UP, 260, 300, makecol(255,0,0), -1);
		else textout_ex(screen, font, TEXT_MAIN_OPTION_LOW, 260, 300, makecol(0,0,0), -1);
		if( selecao==sair ) textout_ex(screen, font, TEXT_MAIN_EXIT_UP, 260, 330, makecol(255,0,0), -1);
		else textout_ex(screen, font, TEXT_MAIN_EXIT_LOW, 260, 330, makecol(0,0,0), -1);

        if( !travado ) {
			if( key[KEY_UP] && !key[KEY_DOWN] && selecao > jogar ) {
				selecao--;
				clear_to_color(screen,makecol(230,230,230));
			}
			if( key[KEY_DOWN] && !key[KEY_UP] && selecao < sair ) {
				selecao++;
				clear_to_color(screen,makecol(230,230,230));
			}
			travado = 1;
		}
		
		if( !key[KEY_UP] && !key[KEY_DOWN] ) travado = 0;
		
		if( !key[KEY_ENTER] ) enter_on=0;
		
		if( key[KEY_ENTER] && !enter_on) {
			
			switch(selecao) {

				case jogar :
		           	play_midi(tema, FALSE);
	           		*speed_counter = 1;
	           		return;
				break;
				case opcao :
		            menu_opcao();
		            enter_on = 1;
		            continue;
				break;
				case sair :
					deinit();
					exit(0);
				break;
			}
		}		
	}
}
END_OF_FUNCTION(menu);

void menu_opcao() {
	int enter_on = key[KEY_ENTER], travado = 0;
	enum { voltar } selecao = voltar;
	
	clear_to_color(screen, makecol(230,230,230));
	
	for(;;) {
		textprintf_ex(screen, font, 200, 170, makecol(0,0,0), -1, TEXT_OPTIONS);
		if( selecao==voltar ) textout_ex(screen, font, TEXT_OPTIONS_BACK_UP, 260, 330, makecol(255,0,0), -1);
		else textout_ex(screen, font, TEXT_OPTIONS_BACK_LOW, 260, 330, makecol(0,0,0), -1);

        if( !travado ) {
			if( key[KEY_UP] && !key[KEY_DOWN] && selecao > voltar ) {
				selecao--;
				clear_to_color(screen,makecol(230,230,230));
			}
			if( key[KEY_DOWN] && !key[KEY_UP] && selecao < voltar ) {
				selecao++;
				clear_to_color(screen,makecol(230,230,230));
			}
			travado = 1;
		}
		
		if( !key[KEY_UP] && !key[KEY_DOWN] ) travado = 0;
		
		if( !key[KEY_ENTER] ) enter_on=0;
		
		if( key[KEY_ENTER] && !enter_on) {
			
			switch(selecao) {

				case voltar :
					clear_to_color(screen, makecol(230,230,230));
					return;
				break;
			}
		}
	}
}
END_OF_FUNCTION(menu_opcao);
