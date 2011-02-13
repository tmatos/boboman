// joguinho bobo
// v. 0005
// por Tiago o Mestre da Armengologia Universal Ilimitada

#ifndef LANG_H

    #define LANG_H
	
	#define PT 1
	#define EN 2

	#define LANGUAGE PT		// Define-se o idioma do jogo aqui

    #if LANGUAGE == PT
        #include"lang/pt.h"
    #elif LANGUAGE == EN
        #include"lang/en.h"
    #else
        #include"lang/en.h"
    #endif

#endif
