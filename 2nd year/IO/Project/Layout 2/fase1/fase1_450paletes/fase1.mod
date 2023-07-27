param N_paletes;

param N_Niveis;

param N_Racks;

set P:={i in 1..N_paletes};

set N:={j in 1..N_Niveis};

set R:={k in 1..N_Racks};

param Palete{i in P, k in 1..4};

param Nivel{j in N, k in 1..3};

var x{i in P, j in N}, binary;

minimize TempoTotal: sum{i in P, j in N}Palete[i,4]*Nivel[j,1]*x[i,j];

subject to Afeta_Paletes{i in P}:sum{j in N}x[i,j]=1;

s.t. Afeta_Niveis{j in N}:sum{i in P}x[i,j]<=1;