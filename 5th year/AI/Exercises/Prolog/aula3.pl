%member(X,[X|T]).
%member(X,[H|T]):-member(X,T).

a2b([],[]).
a2b([a|L1],[b|L2]):-a2b(L1,L2).