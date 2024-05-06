#define npasos 64

#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>
#include <time.h>

unsigned char azar[2000];
unsigned int nazar;

// Rutinas en COLORASM.ASM
extern void poner_menu(unsigned char *,int,int,int,int);
extern void quitar_menu(unsigned char *,int,int,int,int);
extern void poner_modo(int);
extern void caja(int,int,int,int,unsigned char);
extern void espera_redibujo(void);
extern void poner_pantalla_aux(unsigned char *);

// Rutinas en COLOREAR.C
int lista_ficheros(unsigned char *);
int inicio(unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,int,unsigned char *,unsigned char *,unsigned char *,unsigned char *);
unsigned char cargar_pantalla(unsigned char *,unsigned char *,unsigned);
unsigned char cargar_paleta(unsigned char *,unsigned char *);
unsigned char cargar_cursores(unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *);
unsigned char cargar_menus(unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *);
unsigned char compara(unsigned char,unsigned char);
unsigned char cargar_fichero(unsigned char *,int,unsigned char *);
unsigned char tocado(unsigned char);
void error(unsigned char);
void final(void);
void menus(unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,int,unsigned char *,unsigned char *,unsigned char *);
void poner_pantalla_rapido(unsigned char *);
void poner_pantalla(unsigned char *,unsigned char *);
void poner_paleta(unsigned char *);
void aparecer_paleta(unsigned char *);
void modifica(unsigned char,unsigned char);
void modifica2(unsigned char,unsigned char,unsigned char *);
void modifica_rapido(unsigned char,unsigned char);
void cambia_color(unsigned char,unsigned char *,unsigned char *,unsigned char *);
void dibujar_cursor(int,int,unsigned char *,unsigned char *);
void dibujar_mascara(int,int,unsigned char *);
void vaciar_colores(unsigned char *);
void poner_cadena_centrada(unsigned char *,int,unsigned char *);
void poner_cadena(unsigned char *,int,int,unsigned char *);
void poner_letra(unsigned char,int,int,unsigned char *);
void a_minusculas(unsigned char *);
void poner_ojos(int,int);
void oscurecer(unsigned char *);
void oscurecer_rapido(unsigned char *);
void aclarar(unsigned char *);
void poner_paleta_oscura(unsigned char *);
void capturar_paleta(unsigned char *);
void capturar_paleta_completa(unsigned char *);
void poner_paleta_negra(void);
void poner_paleta_negra_rapido(void);
void crear_azar(void);

