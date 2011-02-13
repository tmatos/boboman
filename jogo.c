// joguinho bobo
// v. 0005
// por Tiago o Mestre da Armengologia Universal Ilimitada
//
// Rotinas p/ controle do jogo

#include <allegro.h>
#include <stdio.h>
#include "lang.h"
#include "graficos.h"
#include "msg.h"
#include "jogo.h"

void abertura()
{
	PALETTE pal;
	BITMAP *logo = load_bitmap("img/gran.png",pal);
	SAMPLE *som = load_sample("som/abertura.wav");
	clear_to_color(screen,makecol(64,160,215));
	if(logo) draw_trans_sprite(screen, logo, HRES/2-252, VRES/2-49);
	else textprintf_ex(screen, font, (HRES/2 - strlen(TEXT_PRODUCER)*5), VRES/2, makecol(255,255,255), -1, TEXT_PRODUCER);
	if(som) play_sample( som, 235, 127, 1000, 0 );
	readkey();
	if(som) stop_sample(som);
}
END_OF_FUNCTION(abertura);

// reinicia o jogo, bota ele num inicio
void reseta( TipoCara *cara, TipoCenario *cenario, BITMAP *fundo, BITMAP* fundo2, int *tempo ) {

	PALETTE pal;

	cara->res_x = BLOCKSIZE*1;
	cara->res_y = BLOCKSIZE*4;
	cara->posicao=10;
	cara->altura = VRES-(BLOCKSIZE + cara->res_y);
	cara->velocidade_ver = 0;
	cara->velocidade_hor = 2;
	cara->velocidade_pulo = VELOCIDADE_DO_PULO;
	cara->up_off = 1;
	cara->vidas = NUM_VIDAS;
	cara->img_frente = load_bitmap("img/char.png",pal);
	if( !cara->img_frente ) erro(falta_arquivo, "img/char.png");
	cara->img_virado = load_bitmap("img/char2.png",pal);
	if( !cara->img_virado ) erro(falta_arquivo, "img/char2.png");
	cara->img_frente_anda = load_bitmap("img/char3.png",pal);
	if( !cara->img_frente_anda ) erro(falta_arquivo, "img/char3.png");
	cara->img_virado_anda = load_bitmap("img/char4.png",pal);
	if( !cara->img_virado_anda ) erro(falta_arquivo, "img/char4.png");	
	cara->img = cara->img_frente;
	cara->morte = 0;
	cara->passou = 0;
	cara->energia = 0;

	cenario->coco = load_bitmap("img/coco.png",pal);
	if( !cenario->coco ) erro( falta_arquivo, "img/coco.png" );
	cenario->sol = load_bitmap("img/sol.png",pal);
	if( !cenario->sol ) erro( falta_arquivo, "img/sol.png" );
	cenario->nuvem = load_bitmap("img/nuvem.png",pal);
	if( !cenario->nuvem ) erro( falta_arquivo, "img/nuvem.png" );
	cenario->posicao = 0;

	set_alpha_blender();
	clear_to_color(fundo2,makecol(70,255,255)); // todo: bota isso e o resto numa funcao
	draw_trans_sprite(fundo2, cenario->nuvem, HRES-350-cenario->posicao, VRES-430);
	clear_to_color(fundo,makecol(255,0,255));
	draw_trans_sprite(fundo, cenario->coco, cenario->posicao+HRES-200, VRES-370);
	draw_trans_sprite(fundo, cenario->sol, cenario->posicao+40, 20);

	*tempo = cenario->tempo_max * 100;
}
END_OF_FUNCTION(reseta);

// carrega um fase do arquivo p/ a matrix de sprites e adiciona dados ao cenario
void carrega_fase(char* nome, TipoObjeto matrix[MAXLIN][MAXCOL], TipoCenario *cenario, TipoBloco bloco) {
	int i=0, j=0;
	char objeto;
	
	FILE *arquivo;
	arquivo = fopen(nome,"r");
	if( !arquivo ) erro( falta_arquivo, nome );
	
	fscanf( arquivo, "%d\n%d\n", &cenario->tamanho, &cenario->tempo_max );
	cenario->tamanho *= BLOCKSIZE;
	cenario->tempo_max++;
	
	do {
		objeto = getc(arquivo);
		
		switch(objeto) {
			case '%' :
				matrix[i][j].figura = bloco.pedra;
				matrix[i][j].solido = 1;
			break;
			case '=' :
				matrix[i][j].figura = bloco.chao;
				matrix[i][j].solido = 1;
			break;
			case '#' :
				matrix[i][j].figura = bloco.bloco;
				matrix[i][j].solido = 1;
			break;
			case '*' :
				matrix[i][j].figura = bloco.espinho;
				matrix[i][j].solido = 1;
				matrix[i][j].letal = 1;
			break;
			case 'F' :
				matrix[i][j].figura = bloco.bloco;
				matrix[i][j].solido = 1;
				matrix[i][j].final = 1;
			break;
			case 'p' :
				matrix[i][j].figura = bloco.pilha;
				matrix[i][j].solido = 1;
				matrix[i][j].item = 1;
			break;
			case 'b' :
				matrix[i][j].figura = bloco.monstro;
//				matrix[i][j].solido = 1;
				matrix[i][j].letal = 1;
				matrix[i][j].bicho = (TipoAuto*)malloc( sizeof(TipoAuto) );
				matrix[i][j].bicho->velocidade = 1;
				matrix[i][j].bicho->direcao = direita;
				matrix[i][j].bicho->x = 0/*(j%20)*BLOCKSIZE*/;
				matrix[i][j].bicho->y = i*BLOCKSIZE;
			break;
		}
		
		if( objeto=='\n' && i<14 ) {
			i++;
			j=0;
		}
		else j++;
	} while( !feof(arquivo) );
	
	fclose(arquivo);
}
END_OF_FUNCTION(carrega_fase);

// retorna true se houver colisao entre o cara e algo solido na direcao indicada,
// na matriz de sprites, cada objeto tem um atributo indicando se ele eh solido
// tb identifica colisao com objeto letal e sinaliza morte em cara.morte
int colisao( TipoDirecao direcao, TipoCara *cara, TipoCenario *cenario, TipoObjeto matrix[MAXLIN][MAXCOL] ) {

	int i, j;
	TipoObjeto *ob1, *ob2;

	if( (cara->altura + cara->res_y)/BLOCKSIZE > 14 ) return(0);

	switch(direcao) {

		case abaixo :

			ob1 = &matrix[ (cara->altura + cara->res_y)/BLOCKSIZE ][ (cenario->posicao + cara->posicao)/BLOCKSIZE ];
			ob2 = &matrix[ (cara->altura + cara->res_y)/BLOCKSIZE ][ (cenario->posicao + cara->posicao + cara->res_x)/BLOCKSIZE ];

			if( ob1->solido || ob2->solido ) {

				if( ob1->letal || ob2->letal ) cara->morte = 1;
				if( ob1->final || ob2->final ) cara->passou = 1;
				if( ob1->item || ob2->item ) {
					cara->energia += 20;
					if( ob1->item ) apaga( ob1 );
					else apaga( ob2 );
//					return(0);
				}
				return(1);
			}
			return(0);
		break;

		case acima :
			if( cara->altura < 0 ) return(1);

			ob1 = &matrix[ (cara->altura)/BLOCKSIZE ][ (cenario->posicao + cara->posicao)/BLOCKSIZE ];
			ob2 = &matrix[ (cara->altura)/BLOCKSIZE ][ (cenario->posicao + cara->posicao + cara->res_x)/BLOCKSIZE ];
			
			if( ob1->solido || ob2->solido ) {

				if( ob1->letal || ob2->letal ) cara->morte = 1;
				if( ob1->final || ob2->final ) cara->passou = 1;
				if( ob1->item || ob2->item ) {
					cara->energia += 20;
					if( ob1->item ) apaga( ob1 );
					else apaga( ob2 );
					return(0);
				}
				return(1);
			}
			return(0);
		break;

		case esquerda :
			if( (cara->altura/BLOCKSIZE)*BLOCKSIZE < cara->altura ) j=1;
			else j=0;

			for( i=0 ; i < cara->res_y/BLOCKSIZE ; i++ ) {

				ob1 = &matrix[ cara->altura/BLOCKSIZE + i + j ][ (cenario->posicao + cara->posicao)/BLOCKSIZE ];
				ob2 = &matrix[ cara->altura/BLOCKSIZE + i ][ (cenario->posicao + cara->posicao)/BLOCKSIZE ];

				if( ob1->solido || ob2->solido ) {

					if( ob1->letal || ob2->letal ) cara->morte = 1;
					if( ob1->final || ob2->final ) cara->passou = 1;
					if( ob1->item || ob2->item ) {
						cara->energia += 20;
						if( ob1->item ) apaga( ob1 );
						else apaga( ob2 );
//						return(0);
					}
					return(1);
				}
			}
			return(0);
		break;

		case direita :
			if( (cara->altura/BLOCKSIZE)*BLOCKSIZE < cara->altura ) j=1;
			else j=0;

			for( i=0 ; i < cara->res_y/BLOCKSIZE ; i++ ) {

				ob1 = &matrix[ cara->altura/BLOCKSIZE + i + j ][ (cenario->posicao + cara->posicao + cara->res_x)/BLOCKSIZE ];
				ob2 = &matrix[ cara->altura/BLOCKSIZE + i ][ (cenario->posicao + cara->posicao + cara->res_x)/BLOCKSIZE ];

				if( ob1->solido || ob2->solido ) {

					if( ob1->letal || ob2->letal ) cara->morte = 1;
					if( ob1->final || ob2->final ) cara->passou = 1;
					if( ob1->item || ob2->item ) {
						cara->energia += 20;
						if( ob1->item ) apaga( ob1 );
						else apaga( ob2 );
//						return(0);
					}
					return(1);
				}
			}
			return(0);
		break;
	}
	return(0); // evita warning
}
END_OF_FUNCTION(colisao);

// elimina um abjeto da matriz
void apaga( TipoObjeto *obj ) {
	obj->figura = NULL;
	obj->bicho = NULL;
	obj->solido = 0;
	obj->letal = 0;
	obj->final = 0;
	obj->item = 0;
}
END_OF_FUNCTION(apaga);

// move na matrix um objeto na linha l e coluna c para
// a linha lb e coluna cb, sobrescrevendo
void move_objeto( int l, int c, TipoObjeto matrix[MAXLIN][MAXCOL], int lb, int cb ) {
	matrix[lb][cb] = matrix[l][c];
	apaga( &matrix[l][c] );
}
END_OF_FUNCTION(move_objeto);

// carrega as infos sobre fases do arquivo de def. do jogo para a lista de fases
// a lista tera numero da fase e nome do arquivo dessa fase
void carrega_jogo( char *nome_arq_jogo, TipoFase *lista_fase ) {
	FILE *arq_jogo;
	int i;
	TipoFase *fase;

	arq_jogo = fopen( nome_arq_jogo, "r" );
	if( !arq_jogo ) erro( falta_arquivo, nome_arq_jogo );

	for( i = 1, fase = lista_fase ; !feof(arq_jogo) ; i++ ) {
		fase->numero = i;
		fase->arquivo = (char *)malloc( sizeof(char) * 255 );
		fscanf( arq_jogo, "%s", fase->arquivo );
		fase->proxima = (TipoFase*)malloc( sizeof(TipoFase) );
		fase = fase->proxima;
	}
	fase->arquivo = NULL;
	
	fclose(arq_jogo);
}
END_OF_FUNCTION(carrega_jogo);

// inicializa a matriz de sprites com valores nulos, *IMPORTANTE*
void inicializa( TipoObjeto matrix[MAXLIN][MAXCOL] ) {
	int i,j;

	for( i=0 ; i<MAXLIN ; i++ ) {
		for( j=0 ; j<MAXCOL ; j++ ) {
			matrix[i][j].figura = NULL;
			matrix[i][j].bicho = NULL;
			matrix[i][j].solido = 0;
			matrix[i][j].letal = 0;
			matrix[i][j].final = 0;
			matrix[i][j].item = 0;
		}
	}
}
END_OF_FUNCTION(inicializa);

// carrega algumas configuracoes do jogo para as var. globais respectivas
void carrega_config(int *TELA_CHEIA) {
	FILE *arq_config;
	char palavra[70];

    // defaults
    *TELA_CHEIA = 1;

	arq_config = fopen( "config.cfg", "r" );
	if( !arq_config ) {
        arq_config = fopen( "config.cfg", "w" );
        if( arq_config ) {
            fprintf( arq_config, "TELACHEIA = %d", &TELA_CHEIA );
        }
    }
	else
    do {
		fscanf( arq_config, "%s", &palavra );
		if( !strcmp( palavra, "TELACHEIA" ) ) {
			fscanf( arq_config, " = %d", TELA_CHEIA );
		}
	} while( !feof(arq_config) );

	fclose(arq_config);
}
END_OF_FUNCTION(carrega_config);
