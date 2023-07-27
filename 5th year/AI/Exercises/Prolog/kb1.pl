%% ex1

%mulher(ana).
%mulher(juliana).
%mulher(joana).
%festa.

%% ex2

%feliz(paula).
%ouveMusica(marco).
%ouveMusica(paula):-feliz(paula).
%tocaGuitarra(marco):-ouveMusica(marco).
%tocaGuitarra(paula):-ouveMusica(paula).

%% 5 cláusulas, 2 factos e 3 regras
%% O final de uma cláusula é marcada por um ponto final

%% ex3

%feliz(bruno).
%ouveMusica(miguel).
%tocaGuitarra(bruno):-ouveMusica(bruno),feliz(bruno).
%tocaGuitarra(miguel):-feliz(miguel).
%tocaGuitarra(miguel):-ouveMusica(miguel).

%% sabemos que o bruno é feliz mas não sabemos que ouve musica logo
%% se fizermos tocaGuitarra(bruno) a resposta é falso.

%% V e F -> F // V ou F -> V

%mulher(ana).
%mulher(berta).
%mulher(paula).

%gosta(miguel,ana).
%gosta(bruno,ana).
%gosta(pedro,helena).
%gosta(helena,pedro).

%ciume(X,Y):-gosta(X,Z),gosta(Y,Z).

%% mulher(X).
%% gosta(mario,X),mulher(X).

%% ex 4

elfo(diogo).
bruxa(herminia).
bruxa('Maria').
bruxa(rita).
magico(X):-elfo(X).
magico(X):-feiticeiro(X).
magico(X):-bruxa(X).


