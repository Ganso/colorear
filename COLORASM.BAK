_TEXT SEGMENT BYTE PUBLIC 'CODE'

	ASSUME CS:_TEXT

	PUBLIC _poner_menu
	PUBLIC _quitar_menu
	PUBLIC _poner_modo
	PUBLIC _caja
	PUBLIC _espera_redibujo
	PUBLIC _poner_pantalla_aux

			; PONER MENU

_poner_menu PROC NEAR
push bp
mov bp,sp
push ds
push si
push es
push di
lds si,[bp+4]		; ds:si == sg:of
mov bx,word ptr [BP+12]
mov dx,word ptr [BP+14]
mov di,bx	;di=x
mov bx,dx	;bx=y
shl dx,8	;dx=y*256
shl bx,6	;bx=y*64
add dx,bx	;dx=y*320
add di,dx
mov ax,0a000h
mov es,ax		; es:di == 0xA000:x+y*320
mov cx,word ptr [BP+10]		; Bucle exterior (tamy veces)
mov ah,255		; valor a comparar
repite:
push cx
mov cx,word ptr [BP+8]  ; Bucle interior (tamx veces)
escribe:
lodsb			; al==sg:of , of++
cmp al,ah
jne tmp1
inc di			; al==255 >> di++
jmp tmp2
tmp1:				;
stosb			; ax!=255 >> es:di == ax, di++
tmp2:
loop escribe	; Fin bucle interior
pop cx
add di,320 		; di+=320
sub di,word ptr [BP+8]	; di-=tamx
loop repite		; Fin bucle exterior
pop di
pop es
pop si
pop ds
pop bp
ret
_poner_menu ENDP

			; QUITAR MENU

_quitar_menu PROC NEAR
push bp
mov bp,sp
push ds
push si
push es
push di
mov di,word ptr [BP+12]		;di=x
mov dx,word ptr [BP+14]		;dx=y
mov bx,dx		;bx=y
shl dx,8		;dx=y*256
shl bx,6		;bx=y*64
add dx,bx		;dx=y*320
add di,dx		;di=x+y*320
mov ax,0A000h
mov es,ax			; es:di == 0xA000:x+y*320
lds si,[bp+4]
add si,di			; ds:si == sg:of+x+y*320
mov cx,word ptr [BP+10]		; bucle (tamy veces)
repite2:
push cx
mov cx,word ptr [BP+8]     	; Repite tamx veces...
rep movsb			; ...ds:si ==> es:di
pop cx
add di,320			; di+=320
sub di,word ptr [BP+8]		; di-=tamx
add si,320			; si+=320
sub si,word ptr [BP+8]		; si-=tamx
loop repite2	   		; Fin bucle exterior
pop di
pop es
pop si
pop ds
pop bp
ret
_quitar_menu ENDP

			; PONER MODO GRAFICO

_poner_modo PROC NEAR
push bp
mov bp,sp
mov ax,[bp+4]
int 10h
pop bp
ret
_poner_modo ENDP

			; DIBUJAR UNA CAJA

_caja PROC NEAR
push bp
mov bp,sp
push es
push di
mov ax,0A000h
mov es,ax
mov di,word ptr [BP+4]		;di=x1
mov dx,word ptr [BP+6]		;dx=y1
mov bx,dx		;bx=y1
shl dx,8		;dx=y1*256
shl bx,6		;bx=y1*64
add dx,bx		;dx=y1*320
add di,dx		;es:di==0xA000:x1+y1*320
mov cx,word ptr [BP+10]
sub cx,word ptr [BP+6]
inc cx			;cx=y2-y1+1
mov al,byte ptr [BP+12]
repite3:
push cx
mov cx,word ptr [BP+8]
sub cx,word ptr [BP+4] 	;cx=x2-x1
rep stosb
pop cx
add di,320
add di,[BP+4]
sub di,[BP+8]		;di=di+320-(x2-x1)=di+320-x2+x1
loop repite3
pop di
pop es
pop bp
ret
_caja ENDP

			; ESPERAR UN REDIBUJO

_espera_redibujo PROC NEAR
mov dx,3dah
l1:
in al,dx
and al,08h
jnz l1
l2:
in al,dx
and al,08h
jz l2
ret
_espera_redibujo ENDP

			; FUNCION AUXILIAR PARA PONER PANTALLAS

_poner_pantalla_aux PROC NEAR
push bp
mov bp,sp
sub sp,4
push ds
push si
push es
push di

mov cx,10
ciclo:
mov dx,3c8h
mov al,80h
add al,cl
out dx,al
mov al,10
sub al,cl
mov bl,64
mul bl
mov bl,10
div bl
dec al
inc dx
out dx,al
out dx,al
out dx,al
loop ciclo

lds si,[bp+4]
mov ax,0A000h
mov es,ax
xor di,di
mov cx,200
exter:		;200-cx==linea
push ds
push si
push es
push di
mov [bp-2],cx
push cx
cmp cx,10
jb nadena
mov cx,10
nadena:
mov dl,127
inter:	       	;10-cx==offset
inc dl
mov [bp-4],cx
push cx
mov cx,320
inter2:		;320-cx==columna
lodsb
cmp al,0
je nada
mov al,dl
stosb
jmp algo
nada:
inc di
algo:
loop inter2
pop cx
loop inter
pop cx
pop di
pop es
pop si
pop ds
add si,320
add di,320
call _espera_redibujo
loop exter
pop di
pop es
pop si
pop ds
add sp,4
pop bp
ret
_poner_pantalla_aux ENDP

	_TEXT ENDS


	END

