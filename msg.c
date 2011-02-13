// joguinho bobo
// v. 0005
// por Tiago o Mestre da Armengologia Universal Ilimitada
//
// Rotinas p/ exibicao de informacoes na tela

#include <allegro.h>
#include "msg.h"
#include "graficos.h"
#include "jogo.h"
#include "lang.h"

void exibir( TipoMensagem msg ) {
	
	rectfill( screen, HRES/2-120, VRES/2-40, HRES/2+120, VRES/2+40, makecol(255,255,255 ) );
	
	switch(msg) {
		case tempo_acabou :
			textout_ex(screen, font, TEXT_MSG_TIMEOVER, HRES/2-100, VRES/2-25, makecol(0,0,0), -1);
		break;
		case voce_perdeu :
			textout_ex(screen, font, TEXT_MSG_GAMEOVER, HRES/2-100, VRES/2-25, makecol(0,0,0), -1);
			clear_keybuf();
			readkey();
			return;
		break;
		case opa :
			textout_ex(screen, font, TEXT_MSG_OPS, HRES/2-100, VRES/2-25, makecol(0,0,0), -1);
		break;
		case passou_fase :
			textout_ex(screen, font, TEXT_MSG_PASS, HRES/2-100, VRES/2-25, makecol(0,0,0), -1);
		break;
		case creditos :
			textout_ex(screen, font, TEXT_CREDITS_1, HRES/2-100, VRES/2-25, makecol(0,0,0), -1);
			textout_ex(screen, font, TEXT_CREDITS_2, HRES/2-100, VRES/2-15, makecol(0,0,0), -1);
			textout_ex(screen, font, TEXT_CREDITS_AUTHOR, HRES/2-100, VRES/2+10, makecol(0,0,0), -1);
			rest(5000);
		break;
	}
	
	rest(3000);
	readkey();
}
END_OF_FUNCTION(exibir);

void game_over() {
	exibir(voce_perdeu);
	// e mais coisas
}
END_OF_FUNCTION(game_over);

// dah uma msg de erro e fecha o jogo
void erro( TipoErro id, char *texto ) {

	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	switch(id) {
		case falta_arquivo :
			allegro_message(TEXT_MISSING_FILE, texto);
		break;
		case sem_memoria :
			allegro_message(TEXT_NO_MEMORY);
		break;
		case sem_modo_grafico :
			allegro_message(TEXT_FAIL_GRAPHICS);
		break;
		default : allegro_message(TEXT_ERROR);
	}
	exit(1);
}
END_OF_FUNCTION(erro);

// alguma coisa pra ser feita ou exibida no final do jogo
void zerou() {
	
}
END_OF_FUNCTION(zerou);
