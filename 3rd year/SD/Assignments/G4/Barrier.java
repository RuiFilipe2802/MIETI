import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class Barrier {
    
    private int c = 0;
    private int n ;
    Lock l = new ReentrantLock();
    Condition cond = l.newCondition();

    Barrier(int n){
        this.n = n;
    }

    void wait(){
        l.lock();
        try{
            c += 1;
           if(c<n){ 
            while(c < n){
                cond.await();
            }
        }else{
            cond.signalAll();
        }

        }finally{
            l.unlock();
        }
    }
}
