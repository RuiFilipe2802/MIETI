import java.util.ArrayList;


/**
 * 
 * Classe que define um nível
 * 
 */


public class Nivel implements Comparable {
    
    double largura;
    double altura;
    int rack;
    int nivel;
    int tempoAcesso;
    double espacoLivre;
    ArrayList<Palete> paletes;

    
    public Nivel(double largura, double altura, int rack,int nivel, int tempoAcesso, double espacoLivre,  ArrayList<Palete> paletes){
        this.largura = largura;
        this.altura = altura;
        this.rack = rack;
        this.nivel = nivel;
        this.tempoAcesso = tempoAcesso;
        this.espacoLivre = espacoLivre;
        this.paletes = new ArrayList<>();
    }

    public double getLargura(){
        return this.largura;
    }
    
    public double getAltura(){
        return this.altura;
    }

    public int getRack(){
        return this.rack;
    }

    public int getNivel(){
        return this.nivel;
    }

    public int getTempoAcesso(){
        return this.tempoAcesso;
    }

    public double getEspacoLivre(){
        return this.espacoLivre;
    }
    
    public ArrayList<Palete> getPaletes(){
        return this.paletes;
    }

    public Palete getPalete(int i){
        return this.paletes.get(i);
    }

    public void setLargura(double largura){
        this.largura = largura;
    }

    public void setAltura(double altura){
        this.altura = altura;
    }

    public void setRack(int rack){
        this.rack = rack;
    }

    public void setNivel(int nivel){
        this.nivel = nivel;
    }

    public void setTempoAcesso(int tempoAcesso){
        this.tempoAcesso = tempoAcesso;
    }

    public void setEspacoLivre(double espacoLivre){
        this.espacoLivre = espacoLivre;
    }

    public void setPaletes(Palete palete){
        this.paletes.add(palete);
    }

    public String toString(){
        return "\nRack:"+rack+"\nNivel:"+nivel+"\nAltura:"+altura+"\nPaletes:"+paletes.toString();
        //return "\nRack:"+rack+"\nNivel:"+nivel+"\nAltura:"+altura+"\nEspaço Livre:"+espacoLivre+"\nPaletes:"+paletes.toString()+"\n\n\n";
    }

    @Override
    public int compareTo(Object o) {
        int comparaTempo=((Nivel)o).getTempoAcesso();
        return this.tempoAcesso-comparaTempo;
    }

}