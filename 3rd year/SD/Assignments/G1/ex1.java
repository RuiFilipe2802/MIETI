class Incrementer implements Runnable {
    public void run() {
        final long I = 100;

        for (long i = 0; i < I; i++)
            System.out.println(i);
    }
}

public class ex1 {
    public static void main(String[] args) {
        Incrementer worker = new Incrementer();
        Thread thread[] = new Thread[10];

        for (int i = 0; i < 10; i++) {
            thread[i] = new Thread(worker);
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

        System.out.println("Fim");

    }
}