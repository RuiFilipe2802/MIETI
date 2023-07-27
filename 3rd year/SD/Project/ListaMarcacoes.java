import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.concurrent.locks.ReentrantLock;
import java.io.FileWriter;
import java.io.IOException;

public class ListaMarcacoes {
    private static ArrayList<Marcacao> marcacoes;
    private static ReentrantLock lock;
    Calendar date;
    static Date DataInicial;

    public ListaMarcacoes() {
        marcacoes = new ArrayList<>();
        lock = new ReentrantLock();
        date = Calendar.getInstance();
        date.set(Calendar.HOUR_OF_DAY, 9);
        date.set(Calendar.MINUTE, 0);
        date.set(Calendar.SECOND, 0);
        date.set(Calendar.MILLISECOND, 0);
        long timeInSecs = date.getTimeInMillis();
        DataInicial = new Date(timeInSecs + (1440 * 60 * 1000));
    }

    public void addMarc(int sns, String locall) throws IOException {
        lock.lock();
        Date DataUsar = new Date();
        Date fDate = new Date();
        boolean marc = false;
        int contador = 0;
        for (Marcacao marcacao : marcacoes) {
            if (marcacao.getLocal().equals(locall) && marcacao.getEstado().equals("Desmarcado")) {
                marcacao.setNSns(sns);
                marcacao.setEstado("Marcado");
                marc = true;
                break;
            }
        }
        if (marc == false) {
            if (marcacoes.size() == 0) {
                DataUsar.setTime(DataInicial.getTime() - (20 * 60 * 1000));
            }
            if (marcacoes.size() > 0) {
                for (Marcacao marcacao : marcacoes) {
                    if (marcacao.getLocal().equals(locall)) {
                        contador++;
                        if (DataUsar.compareTo(marcacao.getHora_marcacao()) < 0) {
                            DataUsar = marcacao.getHora_marcacao();
                        }
                    }
                }
                if (contador == 0) {
                    DataUsar.setTime(DataInicial.getTime() - (20 * 60 * 1000));
                }
            }
            try {
                Date ndate = new Date(DataUsar.getTime());
                fDate = ndate;
                Calendar dateNextDay = GregorianCalendar.getInstance();
                dateNextDay.setTime(ndate);
                if ((dateNextDay.get(Calendar.HOUR_OF_DAY)) >= 19) {
                    dateNextDay.set(Calendar.HOUR_OF_DAY, 8);
                    dateNextDay.set(Calendar.MINUTE, 40);
                    dateNextDay.set(Calendar.SECOND, 0);
                    dateNextDay.set(Calendar.MILLISECOND, 0);
                    long timeInSecs = dateNextDay.getTimeInMillis();
                    fDate = new Date(timeInSecs + (1440 * 60 * 1000));
                }
                fDate.setTime(fDate.getTime() + (20 * 60 * 1000));
                Marcacao m = new Marcacao(sns, locall, fDate, "Marcado");
                marcacoes.add(m);
                FileWriter writer = new FileWriter("logs.csv");
                writer.write("SNS   ,Local,Hora Marcacao                ,Estado\n");
                for (Marcacao marcacao : marcacoes) {
                    writer.write(marcacao.toString() + System.lineSeparator());
                }
                writer.close();
            } finally {
                lock.unlock();
            }
        }
    }

    public void desmarcar(int sns) throws IOException {
        lock.lock();
        try {
            for (Marcacao marcacao : marcacoes) {
                if (marcacao.getNSns() == sns) {
                    marcacao.setEstado("Desmarcado");
                    marcacao.setNSns(0);
                    System.out.println(marcacao.getEstado());
                }
            }
            FileWriter writer = new FileWriter("logs.csv");
            writer.write("SNS   ,Local,Hora Marcacao                ,Estado\n");
            for (Marcacao marcacao : marcacoes) {
                writer.write(marcacao.toString() + System.lineSeparator());
            }
            writer.close();
        } finally {
            lock.unlock();
        }
    }

    public boolean verificarSNS(int sns) {
        boolean valido = true;
        lock.lock();
        try {
            for (Marcacao marcacao : marcacoes) {
                if (marcacao.getNSns() == sns) {
                    valido = false;
                }
            }
            return valido;
        } finally {
            lock.unlock();
        }
    }

    public void mostraa() {
        lock.lock();
        try {
            for (Marcacao marcacao : marcacoes) {
                System.out.println(marcacao.getNSns() + "," + marcacao.getLocal() + "," + marcacao.getHora_marcacao()
                        + "," + marcacao.getEstado());
            }
        } finally {
            lock.unlock();
        }

    }
}