
/**
 * Write a description of class Coordenates here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Coordenates
{
    private double x;
    private double y;
    
    private Coordenates(){
    
       x = 0;
       y = 0;
    
    }
    
    public Coordenates(double x, double y){
        
        this.x = x;
        this.y = y;
      
    }
    
    public Coordenates(Coordenates outro){  
        
        this.x = outro.getX();
        this.y = outro.getY();       
    }
    
    public void setX(double x){
    
        this.x = x;

    }
    
    public void setY(double y){
    
        this.y = y;
    
    }
    
    public double getX(){
    
        return this.x;
    
    }
    
    public double getY(){
        
        return this.y;
    
    }
    
    public Coordenates clone(Coordenates outro){
        
        return new Coordenates(this);
        
    }   
    
    public String toString(){
        
    return(this.x+","+this.y);
    
    }
}
