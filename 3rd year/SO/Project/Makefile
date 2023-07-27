all: server test

server: 
	./src/aurrasd etc/aurrasd.comf bin/aurrasd-filters/
client: 

aurrasd.o:
	gcc src/aurrasd.c -o src/aurrasd

aurras.o:
	gcc src/aurras.c -o src/aurras

clean:
	rm obj/* bin/{aurras,aurrasd}

test:
	./src/aurras transform samples/sample-1.m4a out3.mp3 rapido

	