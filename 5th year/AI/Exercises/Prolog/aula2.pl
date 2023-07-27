%digere(X,Y):- comeu(X,Y).
%digere(X,Y):- comeu(X,Z), digere(Z,Y).

%comeu(mosquito,sangue(joao)).
%comeu(sapo,mosquito).
%comeu(cobra,sapo).

filho(ana,bruna).
filho(bruna,carolina).
filho(carolina,diana).
filho(diana,emilia).

descendente(X,Y):- filho(X,Y).
descendente(X,Y):- filho(X,Z),filho(Z,Y).