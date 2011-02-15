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
