/**
 * Write a description of class Client here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Transporter{
    
    private String email;
    private String name;
    private String password;
    private int x;
    private int y;
    
    public Transporter(){
        email = "";
        name = "";
        password = "";
        x = 0;
        y = 0;
    
    }
        
    public Transporter(String email, String name, String password, 
                  int x, int y){            
       this.email = email;
       this.name = name;
       this.password = password;
       this.x = x;
       this.y = y;
    }
    
    public Transporter(Transporter t){
        this.email = t.getEmail();
        this.name = t.getName();
        this.password = t.getPassword();
        this.x = t.getX();
        this.y = t.getY();
    }
    
    public String getEmail(){
        return this.email;
    }
    
    public void setEmail(String email){
        this.email = email;
    }
    
    public String getName(){
        return this.name;
    }
    
    public void setName(String name){
        this.name = name;
    }
    
    public String getPassword(){
        return this.password;
    }
    
    public void setPassword(String password){
        this.password = password;
    }
    
    public int getX(){
        return this.x;
    }
    
    public void setX(int x){
        this.x = x;
    }
    
    public int getY(){
        return this.y;
    }
    
    public void setY(int y){
        this.x = y;
    }
    
    public Transporter clone(){
        return new Transporter(this);
    }
    
    public String toString(){
        return "Name:"+this.name+"Email:"+this.email+"Password"
               +this.password+"X:"+this.x+"Y:"+this.y;
    }   
}