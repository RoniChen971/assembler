assembler: assembler.o ext.o symTable.o FirstTrans.o SecondTrans.o str.o Table.o TxtFunc.o
	gcc -g -Wall -ansi -pedantic \
	assembler.o ext.o FirstTrans.o SecondTrans.o str.o Table.o TxtFunc.o symTable.o\
		-o assembler
assembler.o: assembler.c ext.h TxtFunc.h Table.h Constants.h
	gcc -c -g -Wall -ansi -pedantic assembler.c -o assembler.o
	
ext.o: ext.c ext.h Constants.h Table.h symTable.h  Constants.h
	gcc -c -g -Wall -ansi -pedantic ext.c -o ext.o
	
FirstTrans.o: FirstTrans.c Constants.h Table.h TxtFunc.h ext.h
	gcc -c -g -Wall -ansi -pedantic FirstTrans.c -o FirstTrans.o
	
Table.o: Table.c Table.h Constants.h 
	gcc -c -g -Wall -ansi -pedantic Table.c -o Table.o

TxtFunc.o: TxtFunc.c TxtFunc.h Constants.h
	gcc -c -g -Wall -ansi -pedantic TxtFunc.c -o TxtFunc.o

symTable.o: symTable.c symTable.h Constants.h 
	gcc -c -g -Wall -ansi -pedantic symTable.c -o symTable.o

str.o: str.c Constants.h 
	gcc -c -g -Wall -ansi -pedantic str.c -o str.o	

SecondTrans.o: SecondTrans.c Constants.h  Table.h TxtFunc.h
	gcc -c -g -Wall -ansi -pedantic SecondTrans.c -o SecondTrans.o



clean :
	rm -rf *.o *.ent *.ext *.ob $(objects) assembler
