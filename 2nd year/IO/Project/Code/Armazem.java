import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

/**
 * 
 * Made by:
 * Afonso Miguel Moreira de Queirós Pacheco, A89397
 * Francisco Duarte Gomes Neto, A90297
 * Rui Filipe Ribeiro Freitas, A84121
 * Sandro Teixeira Ribeiro, A85316
 * Tiago João Pereira Ferreira, A85392
 * 
 * MIETI 2019/2020
 * Investigação Operacional
 * 
 */
public class Armazem {

    public static void main(String[] args) {

        ArrayList<Palete> paletes = new ArrayList<>();//Array para guardar as paletes
        ArrayList<Nivel> niveis = new ArrayList<>();// Array para guardar os níveis
        Scanner input = null;
        String filePaletes = "paletes1.txt";
        String fileNiveis = "niveis1.txt";
        int linhas=0;

        linhas=firstMenu(linhas);//Chamada do menu para escolher a instância

        //Carregar ficheiro com as informações das paletes
        try {
            //abre o arquivo
            input = new Scanner(new File(filePaletes));
            String temp;
            //A cada interação, é uma linha do arquivo e atribui-a a temp
            try(BufferedReader br = new BufferedReader(new FileReader(filePaletes))) {
                int lines = 0;
                for (String line; (line = br.readLine()) != null && lines<linhas;)  {
                    lines++;
                    int ref = input.nextInt();
                    double alt = input.nextDouble();
                    double lar = input.nextDouble();
                    int taxa = input.nextInt();
                    Palete palete = new Palete(ref, lar, alt, taxa, false);
                    paletes.add(palete);
                }
            }
        }
        catch (FileNotFoundException el)
        {
            System.out.println("Arquivo não Encontrado!");
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        int i =1;
        //Carregar ficheiro com as informações dos niveis
        try {
            //abre o arquivo
            input = new Scanner(new File(fileNiveis));
            String temp;
            //A cada interação, é uma linha do arquivo e atribui-a a temp
            try(BufferedReader br = new BufferedReader(new FileReader(fileNiveis))) {
                for (String line; (line = br.readLine()) != null; ) {
                    int nivel = input.nextInt();
                    int tempo = input.nextInt();
                    double lar = input.nextDouble();
                    int rack = input.nextInt();                    
                    Nivel nivel1 = new Nivel(lar, 0, rack, i, tempo, 3200, null);
                    niveis.add(nivel1);
                    i++;
                    if(i==11){
                        i=1;
                    }
                }
            }
        }
        catch (FileNotFoundException el)
        {
            System.out.println("Arquivo não Encontrado!");
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        
        //Ordena as peletes por ordem de Taxa de utilização, da maior para a menor.
        Collections.sort(paletes);

        //Ordena os niveis por tempo de acesso, do menor para o maior.
        Collections.sort(niveis);

        int tempoTotal = 0;
        double desperdicio = 0;

        //calcular tempo total
        for(Palete p : paletes){
           if(!p.getUlizada()){
               p.setUtilizada(true);
                 for( Nivel n : niveis){
                    if(n.getEspacoLivre()>= p.largura){
                            n.setPaletes(p);
                            n.setEspacoLivre(n.getEspacoLivre()-p.getLargura());
                            tempoTotal = tempoTotal + p.getTaxaAtualizacao()*n.getTempoAcesso();
                            if(n.getAltura()<p.getAltura()){
                                n.setAltura(p.getAltura());
                            }
                            break;    
                    }
                    
                }
                
            }
        }

        //Calcular desperdicio total
        for(Nivel n : niveis){
            if(n.getPaletes()!=null){
                for(int f = 0; f<n.getPaletes().size(); f++){
                desperdicio = desperdicio +(n.getAltura()-n.getPalete(f).getAltura());
                }
            }
        }


        int j=1;
        //Imprime os niveis com paletes   
        for(Nivel n : niveis){
            if(n.getAltura()!=0 ){
                System.out.println(n.toString()); 
                j++;
               if(j==11){
                   j=1;
               } 
           }
       }  

        System.out.println("\nTempo Total:"+tempoTotal);//Imprime o tempo total
        System.out.println("\nDesperdicio Total:"+desperdicio);//Imprime o desperdicio total
        
    }

    // Menu para escolher a intância
    private static int firstMenu(int linhas){
        int op;
        boolean valido = false;
        Scanner sc = new Scanner (System.in);

        System.out.println("\n---------------------------------------");
        System.out.println("--------------- Armazém ---------------");
        System.out.println("---------------------------------------");
        System.out.println(" 1. MICRO ");
        System.out.println(" 2. MINI ");
        System.out.println(" 3. MÉDIA(1)");
        System.out.println(" 4. MÉDIA(2)");
        System.out.println(" 5. MÉDIA(3)");
        System.out.println(" 6. MÉDIA(4)");
        System.out.println(" 7. MÉDIA(5)");
        System.out.println(" 8. GRANDE");
        System.out.println(" 0. SAIR");
        System.out.println("---------------------------------------");
        System.out.println("---------------------------------------");
        do{
            System.out.println("Opção:");
            op = sc.nextInt();
            valido = (op >= 0) && (op <= 8);
            if(!valido){
                System.out.println("Opção inválida !!!");
            }
        }
        while(!valido);
        System.out.print('\u000C');
        switch(op){
            case 0:
                System.exit(1);
                break;

            case 1:
                linhas = 6;
                break;

            case 2:
                linhas = 60;
                break;

            case 3:
                linhas = 300;
                break;

            case 4:
                linhas = 350;
                break;

            case 5:
                linhas = 400;
                break;

            case 6:
                linhas = 450;
                break;

            case 7:
                linhas = 500;
                break;

            case 8:
                linhas = 1500;
                break;
        }
        return linhas;
    }
}