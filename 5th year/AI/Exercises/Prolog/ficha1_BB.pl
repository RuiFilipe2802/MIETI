%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Inteligência Artificial para as Teleconunicações M(i)ETI
%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Base de Conhecimento com informacao genealogica.
%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Extensao do predicado filho: Filho,Pai -> {V,F}

filho(joao,jose).
filho(jose,manuel).
filho(carlos,jose).
pai(paulo,filipe).
pai(paulo,maria).
avo(antonio,nadia).
neto(nuno,ana).
masculino(joao).
masculino(jose).
feminino(maria).
feminino(joana).


%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Extensao do predicado pai: Pai,Filho -> {V,F}

pai(P,F):- filho(F,P).

%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Extensao do predicado avo: Avo,Neto -> {V,F}

avo(A,N):- filho(N,X),pai(A,X).
neto(N,A):- avo(A,N).

%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Extensao do predicado bisavo: Bisavo,Bisneto -> {V,F}

bisavo(B,N):- pai(B,K),pai(K,M),pai(M,N).


%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Extensao do predicado descendente: Descendente,Ascendente -> {V,F}

descendente(X,Y):- filho(X,Y).
descendente(X,Y):- filho(X,Z),filho(Z,Y).
descendente(X,Y):- filho(X,Z),filho(Z,C),filho(C,Y).


%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Extensao do predicado descendente: Descendente,Ascendente,Grau -> {V,F}


descendente(D,A,1):-
	filho(D,A).
descendente(D,A,G):-
	filho(D,X),
	descendente(X,A,N),
	G is N + 1.
