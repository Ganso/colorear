_TEXT SEGMENT BYTE PUBLIC 'CODE'

	ASSUME CS:_TEXT

	PUBLIC _poner_raton
	PUBLIC _boton_izquierdo
	PUBLIC _boton_derecho
	PUBLIC _donde_cursor

			; ACTIVA EL RATON

_poner_raton PROC NEAR
mov ax,0
int 33h
cmp ax,0
je fin
mov ax,bx
fin:
ret
_poner_raton ENDP

			; DEVUELVE EL ESTADO DEL BOTON IZDO.

_boton_izquierdo PROC NEAR
mov ax,3
int 33h
and bx,1
mov ax,bx
ret
_boton_izquierdo ENDP

			; DEVUELVE EL ESTADO DEL BOTON DCHO.

_boton_derecho PROC NEAR
mov ax,3
int 33h
and bx,2
mov ax,bx
ret
_boton_derecho ENDP

			; DEVUELVE LAS COORDENADAS DEL CURSOR

_donde_cursor PROC NEAR
push bp
mov bp,sp
push es
push di
mov ax,3
int 33h
sar cx,1
mov ax,[BP+6]
mov es,ax
mov di,[BP+4]
mov es:[di],cx
mov ax,[BP+10]
mov es,ax
mov di,[BP+8]
mov es:[di],dx
pop di
pop es
pop bp
ret
_donde_cursor ENDP

	_TEXT ENDS


	END

