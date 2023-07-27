import java.util.Scanner;
/**
 * Write a description of class Main here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Inter
{
    private static Company chief = new Company();
    private static Inter inter = new Inter();
    
    public static void main(String [] args){     
        Company chief = new Company();
        inter.AddClient();
        inter.RemClient();
        inter.AddTransporter();
        inter.RemTransporter();
        
    }
    
    public void AddClient(){    
        String ClientEmail;
        String ClientName;
        String ClientPass;
        String ClientBirth;
        int ClientX;
        int ClientY;
    
        Scanner read = new Scanner(System.in);
        System.out.println("Insert email:");
        ClientEmail = read.nextLine();
        System.out.println("Insert name:");
        ClientName = read.nextLine();
        System.out.println("Insert password:");
        ClientPass = read.nextLine();
        System.out.println("Insert birthday:");
        ClientBirth = read.nextLine();
        System.out.println("Insert X:");
        ClientX = read.nextInt();
        System.out.println("Insert Y:");
        ClientY = read.nextInt();
        
        Client c = new Client(ClientEmail,ClientName,ClientPass,ClientBirth,ClientX, ClientY);
        
        chief.addClient(c.clone());    
    }
    
    public void AddTransporter(){    
        String TransporterEmail;
        String TransporterName;
        String TransporterPass;
        int TransporterX;
        int TransporterY;
    
        Scanner read = new Scanner(System.in);
        System.out.println("Insert email:");
        TransporterEmail = read.nextLine();
        System.out.println("Insert name:");
        TransporterName = read.nextLine();
        System.out.println("Insert password:");
        TransporterPass = read.nextLine();
        System.out.println("Insert X:");
        TransporterX = read.nextInt();
        System.out.println("Insert Y:");
        TransporterY = read.nextInt();
        
        Transporter t = new Transporter(TransporterEmail,TransporterName,TransporterPass,
                                        TransporterX, TransporterY);
        
        chief.addTransporter(t.clone());    
    }
    
    public void RemClient(){
        Scanner read = new Scanner(System.in);
        String ClientName;
        System.out.println("Client name you want to remove:");
        ClientName = read.nextLine();
        chief.remClient(ClientName);     
    }
        
    public void RemTransporter(){
        Scanner read = new Scanner(System.in);
        String TransporterName;
        System.out.println("Transporter name you want to remove:");
        TransporterName = read.nextLine();
        chief.remTransporter(TransporterName);     
    }
}
