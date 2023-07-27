import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

class ServerWorker implements Runnable {
    private Socket socket;
    ArrayList<String> locais = new ArrayList<>();
    ListaMarcacoes listaMarcacoes;

    public ServerWorker(Socket socket, ArrayList<String> locais, ListaMarcacoes listaMarcacoes) {
        this.socket = socket;
        this.locais = locais;
        this.listaMarcacoes = listaMarcacoes;
    }

    @Override
    public void run() {
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            String line;

            while ((line = in.readLine()) != null) {
                String[] result = line.split(" ", 3);
                String command = result[0];
                String localizacao = "";
                int sns = 0;
                boolean validSNS = true;
                boolean validLocal = false;
                if (result.length == 2) {
                    sns = Integer.parseInt(result[1]);
                    if (sns < 100000 || sns > 1000000) {
                        validSNS = false;
                        out.println("Numero de Sns invalido");
                    }
                    if (validSNS == true) {
                        validSNS = listaMarcacoes.verificarSNS(sns);
                        if(validSNS == true){
                            validSNS = false;
                        }else{
                            validSNS = true;
                        }
                        if(validSNS == false){
                            out.println("Numero de Sns não contém marcação");
                        }
                    }
                }
                if (result.length == 3) {
                    localizacao = result[1];
                    for (String loocal : locais) {
                        if (loocal.equals(localizacao)) {
                            validLocal = true;
                        }
                    }
                    if (validLocal == false) {
                        out.println("Local invalido");
                    }
                    sns = Integer.parseInt(result[2]);
                    if (sns < 100000 || sns > 1000000) {
                        validSNS = false;
                        out.println("Numero de Sns invalido");
                    }
                    if (validSNS == true) {
                        validSNS = listaMarcacoes.verificarSNS(sns);
                        if(validSNS == false){
                            out.println("Numero de Sns já contém marcação");
                        }
                    }
                }
                if (command.equals("LOCAIS")) {
                    out.println(locais.toString());
                }
                if (command.equals("AGENDAR") && validLocal == true && validSNS == true) {
                    listaMarcacoes.addMarc(sns, localizacao);
                    System.out.print("\033[H\033[2J");
                    System.out.flush();
                    listaMarcacoes.mostraa();
                    validSNS = false;
                    validLocal = false;
                }
                if (command.equals("DESMARCAR") && validSNS == true) {
                    listaMarcacoes.desmarcar(sns);
                    System.out.print("\033[H\033[2J");
                    System.out.flush();
                    listaMarcacoes.mostraa();
                    validSNS = false;
                }
            }

            socket.shutdownOutput();
            socket.shutdownInput();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

public class server {

    public static void main(String[] args) throws IOException {

        ArrayList<String> locais = new ArrayList<>();
        ListaMarcacoes listaMarcacoes = new ListaMarcacoes();

        locais.add("FAFE");
        locais.add("FAMALICAO");
        locais.add("BRAGA");
        locais.add("GUIMARAES");
        locais.add("PORTO");
        locais.add("LISBOA");
        locais.add("COIMBRA");
        locais.add("FARO");

        ServerSocket serverSocket = new ServerSocket(9998);

        while (true) {
            Socket socket = serverSocket.accept();
            System.out.println("Novo Utente");
            Thread worker = new Thread(new ServerWorker(socket, locais, listaMarcacoes));
            worker.start();
        }
    }
}