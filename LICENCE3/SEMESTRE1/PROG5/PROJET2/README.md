# PROJET DE PROG5 : Simulateur ARM

# Compilation

Avant de compiler :

> make distclean

> autoreconf

Ensuite, reconfigurer :

> ./configure CFLAGS="-Wall -Werror -g"

Puis compiler :

> make

# Exécution

Ouvrez deux terminales : 
- Le premier sera le serveur. Tapez :

> ./arm_simulator

Le simulateur vous donnera des ports à réutiliser plus tard.

- Le deuxième est le client. Allez sur le dossier du projet puis tapez :

> arm-none-eabi-gdb

Puis : 

> file Examples/example1

> target remote localhost:

> load

