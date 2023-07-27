import java.util.concurrent.locks.ReentrantLock;

class Bank {

    private static class Account {
        private int balance;

        Account(int balance) {
            this.balance = balance;
        }

        int balance() {
            return balance;
        }

        boolean deposit(int value) {
            balance += value;
            return true;
        }
    }

    // Our single account, for now
    private Account savings = new Account(0);

    ReentrantLock l = new ReentrantLock();

    // Account balance
    public int balance() {
        l.lock();
        try {
            return savings.balance();
        } finally {
            l.unlock();
        }
    }

    // Deposit
    boolean deposit(int value) {
        l.lock();
        try {
            return savings.deposit(value);
        } finally {
            l.unlock();
        }
    }
}


class Operation implements Runnable {
    private Bank b;

    Operation(Bank b) {
        this.b = b;
    }

    @Override
    public void run() {

        final long I = 1000;
        for (long i = 0; i < I; i++) {
            b.deposit(100);
        }
    }
}

public class ex2 {
    public static void main(String[] args) {
        Bank b = new Bank();
        Operation o = new Operation(b);
        Thread thread[] = new Thread[10];

        for (int i = 0; i < 10; i++) {
            thread[i] = new Thread(o);
        }

        for (int j = 0; j < 10; j++) {
            thread[j].start();
        }

        for (int i = 0; i < 10; i++) {
            try {
                thread[i].join();

            } catch (Exception e) {
            }
        }

        System.out.println(b.balance());
    }

}
