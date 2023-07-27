param H_MAX_RACK;

param N_paletes;

param N_Niveis;

param N_Racks;

set P:={i in 1..N_paletes};

set N:={j in 1..N_Niveis};

set R:={k in 1..N_Racks};

param Palete{i in P,k in 1..4};

param Nivel{j in N,k in 1..3};

var x{i in P, j in N}, binary;

var altura_nivel{j in N}, >=0;

var altura_rack{k in R}, >=0;

minimize TempoTotal: sum{i in P, j in N}Palete[i,4]*Nivel[j,1]*x[i,j];

subject to Afeta_Paletes{i in P}:sum{j in N}x[i,j]=1;

subject to Afeta_Niveis{j in N}:sum{i in P}x[i,j]*Palete[i,3]<=Nivel[j,2];

subject to def_altura_nivel{i in P, j in N}: altura_nivel[j]>=Palete[i,2]*x[i,j];

subject to def_altura_Rack{k in R}: altura_rack[k]=sum{j in N:Nivel[j,3]=k}altura_nivel[j];

subject to def_altura_Rack2{k in R}: altura_rack[k]<=H_MAX_RACK;
