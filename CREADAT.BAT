@echo off
pcx2vcm colores
pcx2vcm menus
pcx2pal colores
copy /q /b colores.pal+colores.vcm+menus.vcm colorear.dat
del /q colores.vcm
del /q menus.vcm
del /q colores.pal
