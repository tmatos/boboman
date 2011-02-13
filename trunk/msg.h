// joguinho bobo
// v. 0005
// por Tiago o Mestre da Armengologia Universal Ilimitada
//
// Rotinas p/ exibicao de informacoes na tela

#ifndef MSG_H

#define MSG_H

typedef enum { falta_arquivo, sem_memoria, sem_modo_grafico } TipoErro;
typedef enum { tempo_acabou, voce_perdeu, opa, passou_fase, creditos } TipoMensagem;

void exibir( TipoMensagem msg );
void game_over();
void erro( TipoErro id, char *texto );
void zerou();

#endif
