param H_MAX_RACK;

param N_paletes;

param N_Niveis;

param N_Racks;

set P:={i in 1..N_paletes};

set N:={i in 1..N_Niveis};

set R:={j in 1..N_Racks};

param Palete{i in P, 1 .. 4};

param Nivel{j in N, 1..3};

var x{i in P, j in N}, binary;

var altura_nivel{j in N}, >=0;

var altura_rack{k in R}, >=0;

var desp_vert {i in P}, >=0;

var aux_fo1, >=0; 

var aux_fo2, >=0;

#minimize TempoTotal: sum{i in P, j in N}Palete[i,4]*Nivel[j,1]*x[i,j];

minimize DespTotal : sum {i in P}desp_vert[i];

subject to Afeta_Paletes{i in P}:sum{j in N}x[i,j]=1;

subject to Afeta_Niveis{j in N}:sum{i in P}x[i,j]*Palete[i,3]<=Nivel[j,2];

subject to def_altura_nivel{i in P, j in N}: altura_nivel[j]>=Palete[i,2]*x[i,j];

subject to def_altura_Rack{k in R}: altura_rack[k]=sum{j in N:Nivel[j,3]=k}altura_nivel[j];

subject to def_altura_Rack2{k in R}: altura_rack[k]<=H_MAX_RACK;

subject to def_desp_vert{i in P, j in N}: x[i,j]=1 ==>(altura_nivel[j]-Palete[i,2]*x[i,j]) = desp_vert[i];

subject to def_aux_fo1: aux_fo1 = sum{i in P, j in N}Palete[i,4]*Nivel[j,1]*x[i,j];

subject to def_aux_fo2: aux_fo2 = sum{i in P}desp_vert[i];

subject to desp_max: aux_fo1 <= 471;

#subject to desp_max: aux_fo2 <= 000;
