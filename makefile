mytree.o : mytree.c
	gcc -c mytree.c
mytree : mytree.o
	gcc mytree.o -o mytree

