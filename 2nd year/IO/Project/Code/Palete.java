
/**
 * 
 * Classe que define uma palete
 * 
 */

public class Palete implements Comparable {

    int referencia;
    double largura;
    double altura;
    int taxaUtilizacao;
    boolean utilizada;

    public Palete(int referencia, double largura, double altura, int taxaUtilizacao, boolean utilizada){
        this.referencia = referencia;
        this.largura = largura;
        this.altura = altura;
        this.taxaUtilizacao = taxaUtilizacao;
        this.utilizada = utilizada;
    }

    public int getReferencia(){
        return this.referencia;
    }

    public double getLargura(){
        return this.largura;
    }
    
    public double getAltura(){
        return this.altura;
    }

    public int getTaxaAtualizacao() {
        return this.taxaUtilizacao;
    }

    public boolean getUlizada(){
        return this.utilizada;
    }

    public void setReferencia(int referencia){
        this.referencia = referencia;
    }

    public void setLargura(double largura){
        this.largura = largura;
    }

    public void setAltura(double altura){
        this.altura = altura;
    }

    public void setTaxaUtilizacao(int taxaUtilizacao){
        this.taxaUtilizacao = taxaUtilizacao;
    }

    public void setUtilizada(boolean utilizada){
        this.utilizada = utilizada;
    }

    public String toString(){
        return String.valueOf(referencia);
    }

@Override
	public int compareTo(Object o) {
        int comparaTaxa=((Palete)o).getTaxaAtualizacao();
        return comparaTaxa-this.taxaUtilizacao;

    }
}