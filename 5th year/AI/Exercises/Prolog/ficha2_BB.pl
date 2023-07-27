%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Inteligência Artificial para as Teleconunicações M(i)ETI
%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Operacoes aritmeticas.
%--------------------------------- - - - - - - - - - -  -  -  -  -   -
%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Extensao do predicado soma: X,Y,Soma -> {V,F}

soma(X,Y,Soma):-
    Soma is X+Y.

%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Extensao do predicado soma: X,Y,Z,Soma -> {V,F}

soma(X,Y,Z,Soma):-
    Soma is X+Y+Z.

%--------------------------------- - - - - - - - - - -  -  -  -  -   -

operacao(adicao,X,4,Ad):-
	Ad is X+4.
operacao(subtracao,X,4,Sub):-
	Sub is X-4.