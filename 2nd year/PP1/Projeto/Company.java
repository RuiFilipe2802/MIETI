import java.util.ArrayList;
import java.util.HashMap;
/**
 * Write a description of class Empresa here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Company{
 
    private HashMap <String,Client> Clients;
    private HashMap <String,Transporter> Transporters;
    
    public Company(){
        this.Clients = new HashMap<String,Client>();
        this.Transporters = new HashMap<String,Transporter>();
    }
    
    public void addClient(Client c){
        this.Clients.put(c.getName(),c.clone());
        System.out.println("**Client successfully added**");
        System.out.println("Email:"+c.getEmail());
        System.out.println("Name:"+c.getName());
        System.out.println("Password:"+c.getPassword());
        System.out.println("Birthday:"+c.getBirthday());
        System.out.println("X:"+c.getX());
        System.out.println("Y:"+c.getY()); 
    }
    
    public void addTransporter(Transporter t){
        this.Transporters.put(t.getName(),t.clone());
        System.out.println("**Transporter successfully added**");
        System.out.println("Email:"+t.getEmail());
        System.out.println("Name:"+t.getName());
        System.out.println("Password:"+t.getPassword());
        System.out.println("X:"+t.getX());
        System.out.println("Y:"+t.getY()); 
    }
    
    public void remClient(String n){
       this.Clients.remove(n);
       System.out.println("**Client sucessfully removed**");       
    }
    
    public void remTransporter(String x){
        this.Transporters.remove(x);
        System.out.println("**Transporter sucessfully removed**");        
    }
}
