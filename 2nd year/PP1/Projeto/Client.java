/**
 * Write a description of class Client here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Client{
    
    private String email;
    private String name;
    private String password;
    private String birthday;
    private int x;
    private int y;
    
    public Client(){
        email = "";
        name = "";
        password = "";
        birthday = "";
        x = 0;
        y = 0;
    
    }
        
    public Client(String email, String name, String password, 
                  String birthday, int x, int y){            
       this.email = email;
       this.name = name;
       this.password = password;
       this.birthday = birthday; 
       this.x = x;
       this.y = y;
    }
    
    public Client(Client c){
        this.email = c.getEmail();
        this.name = c.getName();
        this.password = c. getPassword();
        this.birthday = c.getBirthday();
        this.x = c.getX();
        this.y = c.getY();
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
        
    public String getBirthday(){
        return this.birthday;
    }
    
    public void setBirthday(String birthday){
        this.birthday = birthday;
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
    
    public Client clone(){
        return new Client(this);
    }
    
    public String toString(){
        return "Name:"+this.name+"Email:"+this.email+"Password"
               +this.password+"Birthday"+this.birthday+"X:"+this.x+"Y:"+this.y;
    }   
}
