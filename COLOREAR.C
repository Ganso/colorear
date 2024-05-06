/*
 Para compilar con modelo COMPACTO.
Sobre Borland C++ 3.1 hay que usar la opci¢n de optimizar para
velocidad quitando "Supress redundant loads".

COLOREAR.DAT:
	0000 - COLORES.PAL
	0768 - COLORES.VCM
	6305 - MENUS.VCM
*/

#include "colorear.h"
#include "raton.h"
#include "sb.h"

char cargar_sonido(char *nombre,unsigned char *sonido)
{
int f,tam;
FILE *fich;
fich=fopen(nombre,"rb");
if (!fich) return(0);
fseek(fich,0,SEEK_END);
tam=ftell(fich);
fseek(fich,0,SEEK_SET);
for (f=0;f<=tam-1;f++) *(sonido++)=getc(fich);
return(1);
}

void main(void)
{
unsigned char *pantalla;
unsigned char *colores;
unsigned char *stop,*stop2;
unsigned char *vaciar,*vaciar2;
unsigned char *barra;
unsigned char *jarra;
unsigned char *flecha_a,*flecha_a2;
unsigned char *flecha_b,*flecha_b2;
unsigned char *undo,*undo2;
unsigned char *letra;
unsigned char *fichero;
int nficheros;
unsigned char paleta[768];
unsigned char color;
unsigned char fin;
unsigned char tono;
int x,y;
unsigned char mascara[256],lapiz[256],mano[256],reloj[256];
unsigned char antiguo[378],hacer_undo=0;
unsigned char *sonido;

printf("\n\nCargando...\n\n");

fichero=(unsigned char *)malloc(800);
letra=(unsigned char *)malloc(4800);
colores=(unsigned char *)malloc(17523);
stop=(unsigned char *)malloc(928);
stop2=(unsigned char *)malloc(810);
vaciar=(unsigned char *)malloc(1960);
vaciar2=(unsigned char *)malloc(1696);
barra=(unsigned char *)malloc(11088);
jarra=(unsigned char *)malloc(11730);
flecha_a=(unsigned char *)malloc(187);
flecha_b=(unsigned char *)malloc(187);
flecha_a2=(unsigned char *)malloc(104);
flecha_b2=(unsigned char *)malloc(104);
undo=(unsigned char *)malloc(896);
undo2=(unsigned char *)malloc(864);
//sonido=(unsigned char *)malloc(4176);
pantalla=(unsigned char *)malloc(64000);

//if (!DSPini(0x220)) error(6);
//spkon(0x220);
if (!poner_raton()) error(1);
if (pantalla==NULL) error(2);
if (!(nficheros=lista_ficheros(fichero))) error(3);
if (!cargar_cursores(lapiz,mano,reloj,barra,letra,pantalla)) error(4);
if (!cargar_menus(colores,stop,stop2,vaciar,vaciar2,jarra,flecha_a,flecha_a2,flecha_b,flecha_b2,undo,undo2,pantalla)) error(4);
if (!cargar_paleta("colorear.dat",paleta)) error(4);
//if (!cargar_sonido("arrow.snd",sonido)) error(6);

poner_modo(0x13);
crear_azar();
if (!inicio(pantalla,mano,reloj,jarra,flecha_a,flecha_a2,flecha_b,flecha_b2,barra,fichero,nficheros,letra,stop,stop2,paleta)) final();
fin=0;
modifica_rapido(40,tono=65);
menus(&fin,&tono,pantalla,mano,reloj,letra,colores,stop,stop2,vaciar,vaciar2,barra,jarra,flecha_a,flecha_b,flecha_a2,flecha_b2,undo,undo2,fichero,nficheros,antiguo,&hacer_undo,paleta);

while (!fin) {
	donde_cursor(&x,&y);
	dibujar_cursor(x,y,lapiz,mascara);
	if (boton_izquierdo()) {
		dibujar_mascara(x,y,mascara);
		color=peekb(0xA000,x+y*320);
		color-=((color-2)%6);
		if (!compara(color,tono) && color>=128) {
			hacer_undo=1;
			dibujar_cursor(x,y,reloj,mascara);
			capturar_paleta(antiguo);
			//play(0x220,sonido,4176,8000);
			modifica(color,tono);
			}
		else dibujar_cursor(x,y,lapiz,mascara);
		while(boton_izquierdo()) {}
		}
	if (boton_derecho()) {
		dibujar_mascara(x,y,mascara);
		while (boton_derecho()) {}
		menus(&fin,&tono,pantalla,mano,reloj,letra,colores,stop,stop2,vaciar,vaciar2,barra,jarra,flecha_a,flecha_b,flecha_a2,flecha_b2,undo,undo2,fichero,nficheros,antiguo,&hacer_undo,paleta);
		}
	else dibujar_mascara(x,y,mascara);
	}
while (kbhit()) getch();
final();
}

// Pone la pantalla de inicio

int inicio(unsigned char *pantalla,unsigned char *mano,unsigned char *reloj,unsigned char *jarra,unsigned char *flecha_a,unsigned char *flecha_a2,unsigned char *flecha_b,unsigned char *flecha_b2,unsigned char *barra,unsigned char *fichero,int nficheros,unsigned char *letra,unsigned char *stop,unsigned char *stop2,unsigned char *paleta)
{
unsigned char fin=0,fin2=0;
unsigned char mascara[256];
unsigned char literal[]="poner<la<pantalla<";
unsigned char cadena[30];
unsigned char caracter;
unsigned char color;
int x,y,z,w;
int elegido;
int primero=1;
int opcion=-1;

poner_paleta_negra_rapido();
setmem(pantalla,64000,2);
poner_pantalla_rapido(pantalla);
poner_menu(stop,32,29,282,151);
poner_menu(jarra,115,102,102,17);
poner_menu(flecha_a,17,11,190,49);
poner_menu(flecha_b,17,11,190,77);
z=0;
while (z<10 && primero+z<=nficheros) {
	for (w=0;w<=7;w++) cadena[w]=*(fichero+((primero+z-1)<<3)+w);
	cadena[8]='\0';
	poner_cadena(cadena,114,28+(z<<3),letra);
	z++;
	}
poner_menu(barra,308,36,6,158);
aparecer_paleta(paleta);
modifica_rapido(40,65);
while (!fin2) {
	donde_cursor(&x,&y);
	color=peekb(0xA000,x+y*320);
	poner_ojos(x,y);
	dibujar_cursor(x,y,mano,mascara);

	if (x>=114 && x<=177 && y>=28 && y<=108 && (elegido=primero+(int)((y-28)/8)-1)<nficheros) { if (opcion!=elegido+1) {
		opcion=elegido+1;
		for (w=0;w<=17;w++) cadena[w]=literal[w];
		w=0;
		caracter=*(fichero+(elegido<<3));
		while (w<8 && caracter!='<') {
			cadena[18+w]=caracter;
			w++;
			if (w<8) caracter=*(fichero+(elegido<<3)+w);
			}
		cadena[18+w]='\0';
		caja(8,186,311,191,3);
		poner_cadena_centrada(cadena,185,letra);
		} }
	else if (color>=48 && color<=56 && y<76) { if (opcion!=20) {
		opcion=20;
		caja(8,186,311,191,3);
		poner_cadena_centrada("sube<la<lista<de<archivos",185,letra);
		} }
	else if (color>=48 && color<=56 && y>76) { if (opcion!=21) {
		opcion=21;
		caja(8,186,311,191,3);
		poner_cadena_centrada("baja<la<lista<de<archivos",185,letra);
		} }
	else if (color>=96 && color<=104) { if (opcion!=22) {
		opcion=22;
		caja(8,186,311,191,3);
		poner_cadena_centrada("sale<del<programa",185,letra);
		} }
	else if (opcion) {
		opcion=0;
		caja(8,186,311,191,3);
		poner_cadena_centrada("elige<un<dibujo<para<empezar",185,letra);
		}

	if (boton_izquierdo()) {
		if (opcion>0 && opcion<20) {
			elegido=opcion;
			if (elegido<=nficheros) {
				for (w=0;w<=7;w++) cadena[w]=*(fichero+((elegido-1)<<3)+w);
				cadena[8]='\0';
				a_minusculas(cadena);
				dibujar_mascara(x,y,mascara);
				poner_cadena(cadena,114,28+((elegido-primero)<<3),letra);
				dibujar_cursor(x,y,mano,mascara);
				while (boton_izquierdo());
				fin=0;
				fin2=1;
				}
			}
		else if (opcion==22) {
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,32,29,282,151);
			poner_menu(stop2,30,27,283,152);
			dibujar_cursor(x,y,mano,mascara);
			fin=1;
			fin2=1;
			while (boton_izquierdo());
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,30,27,283,152);
			poner_menu(stop,32,29,282,151);
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=14;z++) espera_redibujo();
			}
		else if (opcion==20 && primero>1) {
			primero--;
			z=0;
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,17,11,190,49);
			poner_menu(jarra,115,102,102,17);
			poner_menu(flecha_a2,13,8,192,50);
			while (z<10 && primero+z<=nficheros) {
				for (w=0;w<=7;w++) cadena[w]=*(fichero+((primero+z-1)<<3)+w);
				cadena[8]='\0';
				poner_cadena(cadena,114,28+(z<<3),letra);
				z++;
				}
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=7;z++) espera_redibujo();
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,13,8,192,50);
			poner_menu(flecha_a,17,11,190,49);
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=6;z++) espera_redibujo();
			}
		else if (opcion==21 && primero<nficheros) {
			primero++;
			z=0;
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,17,11,190,77);
			poner_menu(jarra,115,102,102,17);
			poner_menu(flecha_b2,13,8,192,78);
			while (z<10 && primero+z<=nficheros) {
				for (w=0;w<=7;w++) cadena[w]=*(fichero+((primero+z-1)<<3)+w);
				cadena[8]='\0';
				poner_cadena(cadena,114,28+(z<<3),letra);
				z++;
				}
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=7;z++) espera_redibujo();
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,13,8,192,78);
			poner_menu(flecha_b,17,11,190,77);
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=6;z++) espera_redibujo();
			}
		}
	dibujar_mascara(x,y,mascara);
	}
if (!fin) {
	dibujar_cursor(x,y,reloj,mascara);
	if (!cargar_fichero(fichero,elegido,pantalla)) error(7);
	else {
		poner_pantalla(pantalla,paleta);
		return(1);
		}
	}
return(0);
}

// Pone los men£s

void menus(unsigned char *fin,unsigned char *tono,unsigned char *pantalla,unsigned char *mano,unsigned char *reloj,unsigned char *letra,unsigned char *colores,unsigned char *stop,unsigned char *stop2,unsigned char *vaciar,unsigned char *vaciar2,unsigned char *barra,unsigned char *jarra,unsigned char *flecha_a,unsigned char *flecha_b,unsigned char *flecha_a2,unsigned char *flecha_b2,unsigned char *undo,unsigned char *undo2,unsigned char *fichero,int nficheros,unsigned char *antiguo,unsigned char *hacer_undo,unsigned char *paleta)
{
unsigned char antiguo_bak[378];
unsigned char paleta2[378];
unsigned char fin2=0;
unsigned char mascara[256];
unsigned char literal[]="poner<la<pantalla<";
unsigned char cadena[30];
unsigned char caracter;
unsigned char color;
int x,y,z,w;
int primero=1;
int elegido;
int opcion=-1;
int redibujo=1;

oscurecer(paleta2);
while (!fin2) {
	if (redibujo) {
		espera_redibujo();
		poner_menu(colores,99,177,16,3);
		poner_menu(stop,32,29,282,151);
		poner_menu(vaciar,56,35,214,147);
		poner_menu(jarra,115,102,197,12);
		poner_menu(flecha_a,17,11,285,44);
		poner_menu(flecha_b,17,11,285,72);
		poner_menu(undo,32,28,281,118);
		poner_menu(barra,308,36,6,158);
		redibujo=0;
		z=0;
		while (z<10 && primero+z<=nficheros) {
			for (w=0;w<=7;w++) cadena[w]=*(fichero+((primero+z-1)<<3)+w);
			cadena[8]='\0';
			poner_cadena(cadena,209,23+(z<<3),letra);
			z++;
			}
		}
	donde_cursor(&x,&y);
	color=peekb(0xA000,x+y*320);
	poner_ojos(x,y);
	dibujar_cursor(x,y,mano,mascara);
	if (x>=209 && x<=273 && y>=23 && y<=103 && (elegido=primero+(int)((y-23)/8)-1)<nficheros) { if (opcion!=elegido+1) {
		opcion=elegido+1;
		for (w=0;w<=17;w++) cadena[w]=literal[w];
		w=0;
		caracter=*(fichero+(elegido<<3));
		while (w<8 && caracter!='<') {
			cadena[18+w]=caracter;
			w++;
			if (w<8) caracter=*(fichero+(elegido<<3)+w);
			}
		cadena[18+w]='\0';
		caja(8,186,311,191,3);
		poner_cadena_centrada(cadena,185,letra);
		} }
	else if (color>=48 && color<=56 && y<70) { if (opcion!=20) {
		opcion=20;
		caja(8,186,311,191,3);
		poner_cadena_centrada("sube<la<lista<de<archivos",185,letra);
		} }
	else if (color>=48 && color<=56 && y>70) { if (opcion!=21) {
		opcion=21;
		caja(8,186,311,191,3);
		poner_cadena_centrada("baja<la<lista<de<archivos",185,letra);
		} }
	else if (color>=57 && color<=63) { if (opcion!=22) {
		opcion=22;
		caja(8,186,311,191,3);
		poner_cadena_centrada("deshace<el<ultimo<cambio",185,letra);
		} }
	else if (color>=64 && color<=95) { if (opcion!=23) {
		opcion=23;
		caja(8,186,311,191,3);
		poner_cadena_centrada("elige<el<color",185,letra);
		} }
	else if (color>=96 && color<=104) { if (opcion!=24) {
		opcion=24;
		dibujar_mascara(x,y,mascara);
		caja(8,186,311,191,3);
		poner_cadena_centrada("sale<del<programa",185,letra);
		dibujar_cursor(x,y,mano,mascara);
		} }
	else if (color>=105 && color<=127) { if (opcion!=25) {
		opcion=25;
		dibujar_mascara(x,y,mascara);
		caja(8,186,311,191,3);
		poner_cadena_centrada("quita<los<colores<de<la<pantalla",185,letra);
		dibujar_cursor(x,y,mano,mascara);
		} }
	else if (opcion) {
		opcion=0;
		dibujar_mascara(x,y,mascara);
		caja(8,186,311,191,3);
		poner_cadena_centrada("pulsa<el<boton<derecho<para<colorear",185,letra);
		dibujar_cursor(x,y,mano,mascara);
		}

	if (boton_izquierdo()) {
		if (opcion>0 && opcion<20) {
			elegido=opcion;
			if (elegido<=nficheros) {
				for (w=0;w<=7;w++) cadena[w]=*(fichero+((elegido-1)<<3)+w);
				cadena[8]='\0';
				a_minusculas(cadena);
				dibujar_mascara(x,y,mascara);
				poner_cadena(cadena,209,23+((elegido-primero)<<3),letra);
				dibujar_cursor(x,y,mano,mascara);
				while (boton_izquierdo());
				dibujar_mascara(x,y,mascara);
				dibujar_cursor(x,y,reloj,mascara);
				if (!cargar_fichero(fichero,elegido,pantalla)) error(5);
				else {
					*hacer_undo=0;
					poner_pantalla(pantalla,paleta);
					*tono=65;
					modifica_rapido(40,*tono);
					oscurecer(paleta2);
					redibujo=1;
					}
				}
			}
		else if (opcion==23) {
			while (boton_izquierdo());
			*tono=color;
			modifica_rapido(40,*tono);
			}
		else if (opcion==24) {
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,32,29,282,151);
			poner_menu(stop2,30,27,283,152);
			dibujar_cursor(x,y,mano,mascara);
			*fin=1;
			fin2=1;
			while (boton_izquierdo());
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,30,27,283,152);
			poner_menu(stop,32,29,282,151);
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=14;z++) espera_redibujo();
			}
		else if (opcion==25) {
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,56,35,214,147);
			poner_menu(vaciar2,53,32,216,149);
			dibujar_cursor(x,y,mano,mascara);
			while(boton_izquierdo());
			dibujar_mascara(x,y,mascara);
			dibujar_cursor(x,y,reloj,mascara);
			memcpy(antiguo,paleta2,378);
			vaciar_colores(paleta2);
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,53,32,216,149);
			poner_menu(vaciar,56,35,214,147);
			dibujar_cursor(x,y,mano,mascara);
			}
		else if (opcion==20 && primero>1) {
			primero--;
			z=0;
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,17,11,285,44);
			poner_menu(jarra,115,102,197,12);
			poner_menu(flecha_a2,13,8,287,45);
			while (z<10 && primero+z<=nficheros) {
				for (w=0;w<=7;w++) cadena[w]=*(fichero+((primero+z-1)<<3)+w);
				cadena[8]='\0';
				poner_cadena(cadena,209,23+(z<<3),letra);
				z++;
				}
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=7;z++) espera_redibujo();
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,13,8,287,45);
			poner_menu(flecha_a,17,11,285,44);
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=6;z++) espera_redibujo();
			}
		else if (opcion==21 && primero<nficheros) {
			primero++;
			z=0;
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,17,11,285,72);
			poner_menu(jarra,115,102,197,12);
			poner_menu(flecha_b2,13,8,287,73);
			while (z<10 && primero+z<=nficheros) {
				for (w=0;w<=7;w++) cadena[w]=*(fichero+((primero+z-1)<<3)+w);
				cadena[8]='\0';
				poner_cadena(cadena,209,23+(z<<3),letra);
				z++;
				}
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=7;z++) espera_redibujo();
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,13,8,287,73);
			poner_menu(flecha_b,17,11,285,72);
			dibujar_cursor(x,y,mano,mascara);
			for (z=0;z<=6;z++) espera_redibujo();
			}
		else if (opcion==22) {
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,32,28,281,118);
			poner_menu(undo2,32,26,281,120);
			dibujar_cursor(x,y,mano,mascara);
			while (boton_izquierdo());
			if (*hacer_undo) {
				dibujar_mascara(x,y,mascara);
				dibujar_cursor(x,y,reloj,mascara);
				memcpy(antiguo_bak,antiguo,378);
				memcpy(antiguo,paleta2,378);
				memcpy(paleta2,antiguo_bak,378);
				poner_paleta_oscura(paleta2);
				}
			dibujar_mascara(x,y,mascara);
			quitar_menu(pantalla,32,26,281,120);
			poner_menu(undo,32,28,281,118);
			dibujar_cursor(x,y,mano,mascara);
			}
		}
	if (boton_derecho()) {
		while (boton_derecho());
		poner_pantalla_rapido(pantalla);
		fin2=1;
		}
	if (!fin2 && !redibujo) dibujar_mascara(x,y,mascara);
	}
if (fin2 && !(*fin)) aclarar(paleta2);
}

// Salida con error

void error(unsigned char nerror)
{
poner_modo(0x3);
printf("Error n§%i:\n\n  ",nerror);
switch (nerror) {
	case 1:printf("Necesitas un rat¢n para jugar.");break;
	case 2:printf("Falta memoria convencional.");break;
	case 3:printf("No hay pantallas disponibles.");break;
	case 4:printf("Error en COLOREAR.DAT.");break;
	case 5:printf("Error cargando un dibujo.");break;
	case 6:printf("Sound Blaster no detectada.");break;
	}
printf("\n");
exit(nerror);
}

// Salida sin error

void final(void)
{
poner_paleta_negra();
poner_modo(0x3);
exit(0);
}

// Carga en una matriz la lista de ficheros

int lista_ficheros(unsigned char *fichero)
{
struct ffblk archivo;
int hecho;
int numero=0;
int i,j,k,menor;

hecho=findfirst("*.VCM",&archivo,0);
while (!hecho) {
	numero++;
	i=0;
	while (archivo.ff_name[i]!='.') *(fichero+((numero-1)<<3)+i)=archivo.ff_name[i++];
	for (j=i;j<=7;j++) *(fichero+((numero-1)<<3)+j)='<';
	hecho=findnext(&archivo);
	}
for (j=1;j<=numero-1;j++) {
	for (i=1;i<=numero-j;i++) {
		menor=0;
		k=0;
		while (!menor && k<8) {
			if ( *(fichero+((i-1)<<3)+k) < *(fichero+(i<<3)+k) ) menor=1;
			if ( *(fichero+((i-1)<<3)+k) > *(fichero+(i<<3)+k) ) menor=2;
			k++;
			}
		if (menor==2) {
			for (k=0;k<=7;k++) {
				menor=*(fichero+((i-1)<<3)+k);
				*(fichero+((i-1)<<3)+k)=*(fichero+(i<<3)+k);
				*(fichero+(i<<3)+k)=menor;
				}
			}
		}
	}
return(numero);
}

// Carga una pantalla determinada de la lista de ficheros

unsigned char cargar_fichero(unsigned char *lista,int numero,unsigned char *pantalla)
{
unsigned char nombre[13];
unsigned char caracter;
int i=0;

lista+=(numero-1)<<3;
caracter=*(lista++);
while (caracter!='<' && i<8) {
	if (caracter!='<') nombre[i]=caracter;
	i++;
	caracter=*(lista++);
	}
nombre[i++]='.';
nombre[i++]='V';
nombre[i++]='C';
nombre[i++]='M';
nombre[i++]='\0';
return(cargar_pantalla(nombre,pantalla,0));
}

// Carga pantallas en memoria

unsigned char cargar_pantalla(unsigned char *nombre,unsigned char *pantalla,unsigned offset)
{
FILE *fich;
long f;
int h;
unsigned char dato,dato2;

fich=fopen(nombre,"rb");
if (!fich) return(0);
fseek(fich,offset,SEEK_SET);

f=0;
while (f<64000 && !feof(fich)) {
	dato=getc(fich);
	if (dato>191) {
		dato2=getc(fich);
		f+=(dato-192);
		for (h=0;h<dato-192;h++) *(pantalla++)=dato2;
		}
	else {
		f++;
		*(pantalla++)=dato;
		}
	}
fclose(fich);
if (f!=64000) return(0);
return(1);
}

// Carga una paleta

unsigned char cargar_paleta(unsigned char *nombre,unsigned char *paleta)
{
FILE *fich;
int f;

fich=fopen(nombre,"rb");
if (!fich) return(0);

fread(paleta,768,1,fich);
for (f=0;f<=767;f++) *(paleta++)>>=2;
return(1);
}

// Carga los cursores (y algunas cosas m s)

unsigned char cargar_cursores(unsigned char *lapiz,unsigned char *mano,unsigned char *reloj,unsigned char *barra,unsigned char *letra,unsigned char *pantalla)
{
int f,g,h,i,j;

if (!cargar_pantalla("COLOREAR.DAT",pantalla,768)) return(0);

for (g=1;g<=16;g++) for (f=1;f<=16;f++) *(lapiz++)=*(pantalla+f+g*320);
for (g=1;g<=16;g++) for (f=26;f<=41;f++) *(mano++)=*(pantalla+f+g*320);
for (g=1;g<=16;g++) for (f=51;f<=66;f++) *(reloj++)=*(pantalla+f+g*320);
for (g=158;g<=193;g++) for (f=6;f<=313;f++) *(barra++)=*(pantalla+f+g*320);
f=120; g=19;
for (h=0;h<=74;h++) {
	for (j=g;j<=g+7;j++) for (i=f;i<=f+7;i++) *(letra++)=*(pantalla+i+j*320);
	g+=(f==312)?8:0;
	f=(f==312)?0:f+8;
	}
return(1);
}

// Carga los men£s

unsigned char cargar_menus(unsigned char *colores,unsigned char *stop,unsigned char *stop2,unsigned char *vaciar,unsigned char *vaciar2,unsigned char *jarra,unsigned char *flecha_a,unsigned char *flecha_a2,unsigned char *flecha_b,unsigned char *flecha_b2,unsigned char *undo,unsigned char *undo2,unsigned char *pantalla)
{
int f,g;

if (!cargar_pantalla("COLOREAR.DAT",pantalla,6305)) return(0);

for (g=3;g<=179;g++) for (f=16;f<=114;f++) *(colores++)=*(pantalla+f+g*320);
for (g=151;g<=179;g++) for (f=282;f<=313;f++) *(stop++)=*(pantalla+f+g*320);
for (g=112;g<=138;g++) for (f=148;f<=177;f++) *(stop2++)=*(pantalla+f+g*320);
for (g=147;g<=181;g++) for (f=214;f<=269;f++) *(vaciar++)=*(pantalla+f+g*320);
for (g=78;g<=109;g++) for (f=131;f<=183;f++) *(vaciar2++)=*(pantalla+f+g*320);
for (g=12;g<=113;g++) for (f=197;f<=311;f++) *(jarra++)=*(pantalla+f+g*320);
for (g=44;g<=54;g++) for (f=134;f<=150;f++) *(flecha_a++)=*(pantalla+f+g*320);
for (g=43;g<=53;g++) for (f=155;f<=171;f++) *(flecha_b++)=*(pantalla+f+g*320);
for (g=64;g<=71;g++) for (f=136;f<=148;f++) *(flecha_a2++)=*(pantalla+f+g*320);
for (g=64;g<=71;g++) for (f=152;f<=164;f++) *(flecha_b2++)=*(pantalla+f+g*320);
for (g=118;g<=145;g++) for (f=281;f<=312;f++) *(undo++)=*(pantalla+f+g*320);
for (g=11;g<=36;g++) for (f=140;f<=171;f++) *(undo2++)=*(pantalla+f+g*320);
return(1);
}

// Llena la pantalla desde un buffer r pidamente

void poner_pantalla_rapido(unsigned char *pantalla)
{
movedata (FP_SEG(pantalla),FP_OFF(pantalla),0xA000,0,64000);
}

// Llena la pantalla desde un buffer con un bonito efecto

void poner_pantalla(unsigned char *pantalla,unsigned char *paleta)
{
poner_paleta_negra();
setmem((unsigned char *)MK_FP(0xA000,0),64000,0);
poner_paleta(paleta);
poner_pantalla_aux(pantalla);
poner_paleta(paleta);
poner_pantalla_rapido(pantalla);
}

// Llena la paleta desde un buffer

void poner_paleta(unsigned char *paleta)
{
int f;
espera_redibujo();
for (f=0;f<=255;f++) {
	outportb(0x3c8,f);
	outportb(0x3c9,*(paleta+3*f));
	outportb(0x3c9,*(paleta+3*f+1));
	outportb(0x3c9,*(paleta+3*f+2));
	}
}

// Llena la paleta desde un buffer partiendo de una pantalla oscura

void aparecer_paleta(unsigned char *paleta)
{
int f,cambios=1,paso=0;
char r,g,b;

while (cambios) {
	cambios=0;
	espera_redibujo();
	for (f=0;f<=255;f++) {
		r=*(paleta++);
		g=*(paleta++);
		b=*(paleta++);
		if (r>paso || g>paso || b>paso) {
			outportb(0x3c8,f);
			outportb(0x3c9,(r>paso)?paso:r);
			outportb(0x3c9,(g>paso)?paso:g);
			outportb(0x3c9,(b>paso)?paso:b);
			cambios=1;
			}
		}
	paleta-=768;
	paso++;
	}
}

// Averigua si un color es producto de un tono

unsigned char compara(unsigned char color,unsigned char tono)
{
unsigned char rt1,gt1,bt1;
unsigned char rt2,gt2,bt2;
unsigned char rc1,gc1,bc1;
unsigned char rc2,gc2,bc2;

outportb(0x3c7,tono);
rt1=inportb(0x3c9);
gt1=inportb(0x3c9);
bt1=inportb(0x3c9);

rt2=(rt1>4)?rt1-4:0;
gt2=(gt1>4)?gt1-4:0;
bt2=(bt1>4)?bt1-4:0;

outportb(0x3c7,color);
rc1=inportb(0x3c9);
gc1=inportb(0x3c9);
bc1=inportb(0x3c9);

outportb(0x3c7,color+1);
rc2=inportb(0x3c9);
gc2=inportb(0x3c9);
bc2=inportb(0x3c9);

return(rt1==rc1 && gt1==gc1 && bt1==bc1 && rt2==rc2 && gt2==gc2 && bt2==bc2);
}


// Modifica seis colores con un tono

void modifica(unsigned char color,unsigned char tono)
{
unsigned char r,g,b;
unsigned char r2[6],g2[6],b2[6];
int f;

outportb(0x3c7,tono);
r=inportb(0x3c9);
g=inportb(0x3c9);
b=inportb(0x3c9);

for (f=0;f<=5;f++) {
	r2[f]=(r>(f<<2))?r-(f<<2):0;
	g2[f]=(g>(f<<2))?g-(f<<2):0;
	b2[f]=(b>(f<<2))?b-(f<<2):0;
	}
cambia_color(color,r2,g2,b2);
}

// Modifica seis colores con un tono  desde los men£s

void modifica2(unsigned char color,unsigned char tono,unsigned char *paleta)
{
unsigned char r,g,b;
unsigned char r2[6],g2[6],b2[6];
int f;

if (tono>95) {
	outportb(0x3c7,tono-32);
	r=inportb(0x3c9);
	g=inportb(0x3c9);
	b=inportb(0x3c9);

	paleta+=(color-128)*3;
	for (f=0;f<=5;f++) {
		*(paleta++)=r;
		*(paleta++)=g;
		*(paleta++)=b;
		r2[f]=r-21;
		g2[f]=g-21;
		b2[f]=b-21;
		}
	cambia_color(color,r2,g2,b2);
	}
else {
	outportb(0x3c7,tono);
	r=inportb(0x3c9);
	g=inportb(0x3c9);
	b=inportb(0x3c9);

paleta+=(color-128)*3;
	*(paleta++)=r;
	*(paleta++)=g;
	*(paleta++)=b;

	for (f=1;f<=5;f++) {
		*(paleta++)=(r>4*f)?r-4*f:0;
		*(paleta++)=(g>4*f)?g-4*f:0;
		*(paleta++)=(b>4*f)?b-4*f:0;
		}
	r=(r>21)?r-21:0;
	g=(g>21)?g-21:0;
	b=(b>21)?b-21:0;

	for (f=0;f<=5;f++) {
		r2[f]=(r>4*f)?r-4*f:0;
		g2[f]=(g>4*f)?g-4*f:0;
		b2[f]=(b>4*f)?b-4*f:0;
		}
	cambia_color(color,r2,g2,b2);
	}
}

// Modifica seis colores con un tono r pidamente

void modifica_rapido(unsigned char color,unsigned char tono)
{
unsigned char r,g,b;
int f;

outportb(0x3c7,tono);
r=inportb(0x3c9);
g=inportb(0x3c9);
b=inportb(0x3c9);

espera_redibujo();
for (f=0;f<=5;f++) {
	outportb(0x3c8,color+f);
	outportb(0x3c9,(r>4*f)?r-4*f:0);
	outportb(0x3c9,(g>4*f)?g-4*f:0);
	outportb(0x3c9,(b>4*f)?b-4*f:0);
	}
}

// Cambia lentamente un tono por otro

void cambia_color(unsigned char color,unsigned char *r,unsigned char *g,unsigned char *b)
{
unsigned char r2[6],g2[6],b2[6],f;
int h,rv[6],gv[6],bv[6];
for (f=0;f<=5;f++) {
	outportb(0x3c7,color+f);
	r2[f]=inportb(0x3c9);
	g2[f]=inportb(0x3c9);
	b2[f]=inportb(0x3c9);
	rv[f]=(r[f]-r2[f]);
	gv[f]=(g[f]-g2[f]);
	bv[f]=(b[f]-b2[f]);
	}
for (h=0;h<npasos;h++) {
	espera_redibujo();
	for (f=0;f<=5;f++) {
		outportb(0x3c8,color+f);
		outportb(0x3c9,r2[f]+(rv[f]*h)/npasos);
		outportb(0x3c9,g2[f]+(gv[f]*h)/npasos);
		outportb(0x3c9,b2[f]+(bv[f]*h)/npasos);
		}
	}
espera_redibujo();
for (f=0;f<=5;f++) {
	outportb(0x3c8,color+f);
	outportb(0x3c9,r[f]);
	outportb(0x3c9,g[f]);
	outportb(0x3c9,b[f]);
	}
}

// Averigua si es la primera vez que se cambia un color

unsigned char tocado(unsigned char color)
{
unsigned char r,g,b;
unsigned char r2,g2,b2;

outportb(0x3c7,color);
r=inportb(0x3c9);
g=inportb(0x3c9);
b=inportb(0x3c9);

outportb(0x3c7,color+1);
r2=inportb(0x3c9);
g2=inportb(0x3c9);
b2=inportb(0x3c9);

return (r!=r2 || g!=g2 || b!=b2);
}

// Dibuja el cursor gr fico y salva el fondo

void dibujar_cursor(int x,int y,unsigned char *cursor,unsigned char *mascara)
{
int sgc=FP_SEG(cursor),ofc=FP_OFF(cursor);
int sgm=FP_SEG(mascara),ofm=FP_OFF(mascara);
int topea,topeb;

asm cmp x,303
asm ja  mayortopea
asm mov topea,16
asm jmp fintopea
mayortopea:
asm mov ax,320
asm sub ax,x
asm mov topea,ax
fintopea:

asm cmp y,183
asm ja  mayortopeb
asm mov topeb,16
asm jmp fintopeb
mayortopeb:
asm mov ax,200
asm sub ax,y
asm mov topeb,ax
fintopeb:

asm {
	push ds
	push si
	push es
	push di
	push ax
	push bx
	push cx
	push dx
	mov es,sgm
	mov di,ofm
	mov bx,x
	mov dx,y
	mov si,bx
	mov bx,dx
	shl dx,8
	shl bx,6
	add dx,bx
	add si,dx
	mov ax,0xa000
	mov ds,ax
	mov cx,topeb
}
repitem:
asm {
	push cx
	mov cx,topea
	}
escribem:
asm {
	lodsb
	stosb
	loop escribem
	cmp topea,16
	je vuelvem
	mov cx,16
	sub cx,topea
	}
derecham:
asm {
	inc di
	loop derecham
	}
vuelvem:
asm {
	pop cx
	add si,320
	sub si,topea
	loop repitem

	mov ds,sgc
	mov si,ofc
	mov bx,x
	mov dx,y
	mov di,bx
	mov bx,dx
	shl dx,8
	shl bx,6
	add dx,bx
	add di,dx
	mov ax,0xa000
	mov es,ax
	mov cx,topeb
}
repite:
asm {
	push cx
	mov cx,topea
	}
escribe:
asm {
	lodsb
	cmp al,31
	jne tmp1
	inc di
	jmp tmp2
	}
tmp1:
asm stosb
tmp2:
asm {
	loop escribe
	cmp topea,16
	je vuelve
	mov cx,16
	sub cx,topea
	}
derecha:
asm inc si
asm loop derecha
vuelve:
asm {
	pop cx
	add di,320
	sub di,topea
	loop repite
	pop dx
	pop cx
	pop bx
	pop ax
	pop di
	pop es
	pop si
	pop ds
	}
}

// Restaura el cursor con una m scara

void dibujar_mascara(int x,int y,unsigned char *mascara)
{
int topea,topeb;
unsigned sg=FP_SEG(mascara),of=FP_OFF(mascara);

asm cmp x,303
asm ja  mayortopea
asm mov topea,16
asm jmp fintopea
mayortopea:
asm mov ax,320
asm sub ax,x
asm mov topea,ax
fintopea:

asm cmp y,183
asm ja  mayortopeb
asm mov topeb,16
asm jmp fintopeb
mayortopeb:
asm mov ax,200
asm sub ax,y
asm mov topeb,ax
fintopeb:

espera_redibujo();

asm {
	push ds
	push si
	push es
	push di
	push ax
	push bx
	push cx
	push dx
	mov ds,sg
	mov si,of
	mov bx,x
	mov dx,y
	mov di,bx
	mov bx,dx
	shl dx,8
	shl bx,6
	add dx,bx
	add di,dx
	mov ax,0xa000
	mov es,ax
	mov cx,topeb
	}
repite:
asm push cx
asm mov cx,topea
escribe:
asm {
	lodsb
	stosb
	loop escribe
	cmp topea,16
	je vuelve
	mov cx,16
	sub cx,topea
	}
derecha:
asm inc si
asm loop derecha
vuelve:
asm {
	pop cx
	add di,320
	sub di,topea
	loop repite
	pop dx
	pop cx
	pop bx
	pop ax
	pop di
	pop es
	pop si
	pop ds
	}
}

// Vacia la pantalla de colores (fundido a blanco)

void vaciar_colores(unsigned char *paleta)
{
unsigned char r,g,b;
int f,h;

for (f=0;f<=377;f++) *(paleta++)=63;
for (h=0;h<=41;h++) {
	espera_redibujo();
	for (f=128;f<=253;f++) {
		outportb(0x3c7,f);
		r=inportb(0x3c9);
		g=inportb(0x3c9);
		b=inportb(0x3c9);
		outportb(0x3c8,f);
		outportb(0x3c9,(r<43)?r+1:43);
		outportb(0x3c9,(g<43)?g+1:43);
		outportb(0x3c9,(b<43)?b+1:43);
		}
	}
}

// Pone una cadena centrada

void poner_cadena_centrada(unsigned char *cadena,int y,unsigned char *tipo)
{
int f=0;

while (cadena[f]!='\0') f++;
poner_cadena(cadena,(320-(f<<3))>>1,y,tipo);
espera_redibujo();
}

// Pone una cadena letra a letra

void poner_cadena(unsigned char *cadena,int x,int y,unsigned char *tipo)
{
int f=0;

while (cadena[f]!='\0') {
	poner_letra(cadena[f],x+f*8,y,tipo);
	f++;
	}
}

// Pone una letra desde un tipo de letra

void poner_letra(unsigned char letra,int x,int y,unsigned char *tipo)
{
unsigned sg=FP_SEG(tipo),of=FP_OFF(tipo);

asm {
	push ds
	push si
	push es
	push di
	push ax
	push bx
	push cx
	push dx
	mov ds,sg
	xor bh,bh
	mov bl,letra
	sub bx,48
	shl bx,6
	add bx,of
	mov si,bx			// ds:si == sg:off+(letra-48)<<6
	mov bx,x
	mov dx,y
	mov di,bx	//di=x
	mov bx,dx	//bx=y
	shl dx,8    	//dx=y*256
	shl bx,6	//bx=y*64
	add dx,bx	//dx=y*320
	add di,dx
	mov ax,0xA000
	mov es,ax			// es:di == 0xA000,x+y*320
	mov cx,8			// Bucle exterior (8 veces)
	mov ah,0			// valor a comparar
	}
repite:
asm push cx
asm mov cx,8				// Bucle interior (8 veces)
escribe:
asm {
	lodsb				// al == sg:of , of++
	cmp al,ah
	jne tmp1
	inc di				// al!=255 >> di++
	jmp tmp2
	}
tmp1:
asm stosb				// al==255 >> es:di == al , di++
tmp2:
asm {
	loop escribe			// Fin bucle interior
	pop cx
	add di,312			// di+=312
	loop repite
	pop dx
	pop cx
	pop bx
	pop ax
	pop di
	pop es
	pop si
	pop ds
	}
}

// Pasa una cadena a min£sculas

void a_minusculas(unsigned char *cadena)
{
int f=0;

while (cadena[f]!='\0') cadena[f++]+=(cadena[f]>=65 && cadena[f]<=90)?32:0;
}

// Pone los ojos de los ni¤os

void poner_ojos(int x,int y)
{
int lado=(x<146)?((y<166)?0:1):((y<166)?3:2);

if (azar[nazar]) switch(lado) {
	case 0: pokeb(0xA000,53264,11); pokeb(0xA000,53265,2);
		pokeb(0xA000,53267,11); pokeb(0xA000,53268,2);
		pokeb(0xA000,53584,3);  pokeb(0xA000,53585,3);
		pokeb(0xA000,53587,3);  pokeb(0xA000,53588,3);
		break;
	case 1: pokeb(0xA000,53264,3);  pokeb(0xA000,53265,3);
		pokeb(0xA000,53267,3);  pokeb(0xA000,53268,3);
		pokeb(0xA000,53584,11); pokeb(0xA000,53585,2);
		pokeb(0xA000,53587,11); pokeb(0xA000,53588,2);
		break;
	case 2: pokeb(0xA000,53264,3);  pokeb(0xA000,53265,3);
		pokeb(0xA000,53267,3);  pokeb(0xA000,53268,3);
		pokeb(0xA000,53584,2);  pokeb(0xA000,53585,11);
		pokeb(0xA000,53587,2);  pokeb(0xA000,53588,11);
		break;
	case 3: pokeb(0xA000,53264,2);  pokeb(0xA000,53265,11);
		pokeb(0xA000,53267,2);  pokeb(0xA000,53268,11);
		pokeb(0xA000,53584,3);  pokeb(0xA000,53585,3);
		pokeb(0xA000,53587,3);  pokeb(0xA000,53588,3);
	}
else {		pokeb(0xA000,53264,23);  pokeb(0xA000,53265,23);
		pokeb(0xA000,53267,23);  pokeb(0xA000,53268,23);
		pokeb(0xA000,53584,23);  pokeb(0xA000,53585,23);
		pokeb(0xA000,53587,23);  pokeb(0xA000,53588,23);
	}

nazar=(nazar>1996)?0:nazar+1;

lado=(x<185)?((y<166)?0:1):((y<166)?3:2);

if (azar[nazar]) switch(lado) {
	case 0: pokeb(0xA000,53303,11); pokeb(0xA000,53304,2);
		pokeb(0xA000,53306,11); pokeb(0xA000,53307,2);
		pokeb(0xA000,53623,3);  pokeb(0xA000,53624,3);
		pokeb(0xA000,53626,3);  pokeb(0xA000,53627,3);
		break;
	case 1: pokeb(0xA000,53303,3);  pokeb(0xA000,53304,3);
		pokeb(0xA000,53306,3);  pokeb(0xA000,53307,3);
		pokeb(0xA000,53623,11); pokeb(0xA000,53624,2);
		pokeb(0xA000,53626,11); pokeb(0xA000,53627,2);
		break;
	case 2: pokeb(0xA000,53303,3);  pokeb(0xA000,53304,3);
		pokeb(0xA000,53306,3);  pokeb(0xA000,53307,3);
		pokeb(0xA000,53623,2);  pokeb(0xA000,53624,11);
		pokeb(0xA000,53626,2);  pokeb(0xA000,53627,11);
		break;
	case 3: pokeb(0xA000,53303,2);  pokeb(0xA000,53304,11);
		pokeb(0xA000,53306,2);  pokeb(0xA000,53307,11);
		pokeb(0xA000,53623,3);  pokeb(0xA000,53624,3);
		pokeb(0xA000,53626,3);  pokeb(0xA000,53627,3);
	}
else {		pokeb(0xA000,53303,23); pokeb(0xA000,53304,23);
		pokeb(0xA000,53306,23); pokeb(0xA000,53307,23);
		pokeb(0xA000,53623,23);  pokeb(0xA000,53624,23);
		pokeb(0xA000,53626,23);  pokeb(0xA000,53627,23);
		}


nazar++;
}

// Oscurece un poco la pantalla

void oscurecer(unsigned char *paleta)
{
int a,b;
unsigned char c;

for (b=0;b<=125;b++) {
	outportb(0x3c7,b+128);
	*(paleta++)=inportb(0x3c9);
	*(paleta++)=inportb(0x3c9);
	*(paleta++)=inportb(0x3c9);
	}

for (a=1;a<=20;a++) {
	espera_redibujo();
	paleta-=378;
	for (b=0;b<=125;b++) {
		outportb(0x3c8,b+128);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		}
	}
}

// Oscurece un poco la pantalla r pidamente

void oscurecer_rapido(unsigned char *paleta)
{
int a,b;
unsigned char c;

for (b=0;b<=125;b++) {
	outportb(0x3c7,b+128);
	*(paleta++)=inportb(0x3c9);
	*(paleta++)=inportb(0x3c9);
	*(paleta++)=inportb(0x3c9);
	}

for (a=1;a<=20;a++) {
	paleta-=378;
	for (b=0;b<=125;b++) {
		outportb(0x3c8,b+128);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		}
	}
}

// vuelve a alcarar la pantalla

void aclarar(unsigned char *paleta)
{
int a,b;
unsigned char c;

paleta+=378;
for (a=19;a>=0;a--) {
	espera_redibujo();
	paleta-=378;
	for (b=0;b<=125;b++) {
		outportb(0x3c8,b+128);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		c=*(paleta++);
		outportb(0x3c9,(c>a)?(c-a):0);
		}
	}
}

// Actualiza la pantalla estando en los menus

void poner_paleta_oscura(unsigned char *paleta)
{
int d;
int r,g,b;
int r2,g2,b2;
unsigned int cambios=1;

while (cambios) {
	espera_redibujo();
	cambios=0;
	for (d=0;d<=125;d++) {
		outportb(0x3c7,d+128);
		r=inportb(0x3c9);
		g=inportb(0x3c9);
		b=inportb(0x3c9);
		r2=*(paleta++);
		g2=*(paleta++);
		b2=*(paleta++);
		r2=(r2>20)?r2-20:0;
		g2=(g2>20)?g2-20:0;
		b2=(b2>20)?b2-20:0;
		if (r!=r2 || g!=g2 || b!=b2) {
			outportb(0x3c8,d+128);
			outportb(0x3c9,(r>r2)?r-1:(r<r2)?r+1:r);
			outportb(0x3c9,(g>g2)?g-1:(g<g2)?g+1:g);
			outportb(0x3c9,(b>b2)?b-1:(b<b2)?b+1:b);
			cambios++;
			}
		}
	paleta-=378;
	}
}

// Captura la paleta desde el 128 al 253

void capturar_paleta(unsigned char *paleta)
{
int b;

for (b=0;b<=125;b++) {
	outportb(0x3c7,b+128);
	*(paleta++)=inportb(0x3c9);
	*(paleta++)=inportb(0x3c9);
	*(paleta++)=inportb(0x3c9);
	}
}

// Captura la paleta completamente

void capturar_paleta_completa(unsigned char *paleta)
{
int b;

for (b=0;b<=255;b++) {
	outportb(0x3c7,b);
	*(paleta++)=inportb(0x3c9);
	*(paleta++)=inportb(0x3c9);
	*(paleta++)=inportb(0x3c9);
	}
}

// Hace un fundido a negro

void poner_paleta_negra(void)
{
char r,g,b;
int f,cambios=1;

while (cambios) {
	cambios=0;
	espera_redibujo();
	for (f=0;f<=255;f++) {
		outportb(0x3c7,f);
		r=inportb(0x3c9);
		g=inportb(0x3c9);
		b=inportb(0x3c9);
		if (r || g || b) {
			outportb(0x3c8,f);
			outportb(0x3c9,(r)?r-1:0);
			outportb(0x3c9,(g)?g-1:0);
			outportb(0x3c9,(b)?b-1:0);
			cambios=1;
			}
		}
	}
}

// Pone la paleta negra r pidamente

void poner_paleta_negra_rapido(void)
{
int f;

espera_redibujo();
for (f=0;f<=255;f++) {
	outportb(0x3c8,f);
	outportb(0x3c9,0);
	outportb(0x3c9,0);
	outportb(0x3c9,0);
	}
}

// Crear una tabla de n£mero aleatorios

void crear_azar(void)
{
int f;
randomize();
for (f=0;f<=1999;f++) {
	if (f>2) {
		if (!azar[f-2]) azar[f]=!random(5);
		else azar[f]=random(150);
		}
	else azar[f]=random(150);
	}
nazar=0;
}
