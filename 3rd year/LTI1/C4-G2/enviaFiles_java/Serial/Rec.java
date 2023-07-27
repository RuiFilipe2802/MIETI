import com.fazecast.jSerialComm.SerialPort;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.Scanner;

import static java.lang.Thread.sleep;

public class Rec {
    public static void main(String[] args) throws IOException, InterruptedException {

        long inicio;
        long fim;
        float tempo;
        int op;
        boolean valido;


        SerialPort[] ports = SerialPort.getCommPorts();//Array para guardar as portas série disponíveis
        if (ports.length == 0){
            System.out.println("No Ports Available");
            sleep(2000);
            System.exit(0);
        }
        System.out.println("Choose Port:");
        int i = 1;
        for (SerialPort port : ports) {     //Seleção da porta série a usar
            System.out.println(i++ + ". " + port.getSystemPortName());
        }
        Scanner sd = new Scanner(System.in);
        int sp = sd.nextInt();
        SerialPort port = ports[sp - 1];

        if (port.openPort()) {
            System.out.println("Port Opened");
        } else {
            System.out.println("Port Closed");

        }
        port.setComPortParameters(115200, 8, 1, 0);//Configuração da porta série

        //Menu principal
        Scanner s = new Scanner(System.in);
        System.out.println("\n----------------------------------------");
        System.out.println("------------ LTI I - FASE 1 ------------");
        System.out.println("----------------------------------------");
        System.out.println(" 1. Receive Text File ");
        System.out.println(" 2. Receive Image File ");
        System.out.println(" 3. CHAT ");
        System.out.println(" 4. Exit ");
        System.out.println("----------------------------------------");
        System.out.println("----------------------------------------");
        do {
            System.out.println("Option:");
            op = s.nextInt();
            valido = (op >= 1) && (op <= 4);
            if (!valido) {
                System.out.println("Invalid Option !!!");
            }
        }
        while (!valido);
        switch (op) {
            case 1:
                receberFicheiroTexto(port);             //Método para receber
                port.closePort();                       //Fecha a porta série
                break;

            case 2:
                receberImagem(port);
                port.closePort();                       //Fecha a porta série
                break;

            case 3:
                while (true) {
                    recebeChat(port);
                    enviaChat(port);
                }
            case 4:
                System.exit(1);                   //Sai do programa
                break;
        }
    }

    //Método para enviar o ficheiro de texto
    public static void enviaChat(SerialPort port) throws IOException, InterruptedException {
        int j, x=0;
        byte[] bytesFromFile ;
        System.out.print("EU:");
        Scanner sc = new Scanner(System.in);
        String msg = sc.nextLine();
        bytesFromFile = msg.getBytes();
        long inicio = System.currentTimeMillis();    //Começa a contar o tempo de envio
        float nPac;
        int nPacotes;
        nPac = (float) Math.ceil(((bytesFromFile.length)/30.0));
        nPacotes= (int) nPac;
        for(int i=0;i<nPacotes;i++) {
            byte[] bytesFromF = new byte[31];
            if(i == (nPacotes-1)){
                bytesFromF[0] = (byte) 0b0110010;
            }
            else {
                bytesFromF[0] = (byte) 0b00110001;
            }
            for (j = 0; j < 30; j++) {
                if (j + x == bytesFromFile.length) {
                    break;
                }
                bytesFromF[j + 1] = bytesFromFile[j + x];
            }
            x += 30;
            port.writeBytes(bytesFromF, 31);//Envia para a porta série o array de bytes
            byte[] bufferR = new byte[2];//Array para guardar os bytes recebidos pela porta série
            while (port.bytesAvailable() < 2000000) {//Espera pelo envio do último byte
                port.readBytes(bufferR, 2);
                if (bufferR[0] == (-114) && bufferR[1] == (-114)) {
                    break;
                }
            }
        }
        long fim = System.currentTimeMillis();       //Para de contar o tempo de envio
        float tempo = (fim - inicio);                 //Calcula o tempo de envio
    }

    //Método para receber chat
    public static void recebeChat(SerialPort port) throws IOException {
        String str;
        byte[] buffer = new byte[30];
        while (port.bytesAvailable() < 30) {
            //System.out.println(port.bytesAvailable());
        }
        port.readBytes(buffer, 30);
        str = new String(buffer);
        System.out.println("Msg Recebida:" +str);
    }

    //Método para receber o ficheiro de texto
    public static void receberFicheiroTexto(SerialPort port) throws IOException {
        int nBytes;
        System.out.println("NUMBER OF BYTES:");
        Scanner sc = new Scanner(System.in);
        nBytes = sc.nextInt();
        String str;
        System.out.println(nBytes);
        byte[] buffer = new byte[nBytes];
        while (port.bytesAvailable() < nBytes) {
            System.out.println(port.bytesAvailable());
        }
        port.readBytes(buffer, nBytes);
        Files.write(new File("C:\\Users\\tiago\\Desktop\\rece1.txt").toPath(), buffer);//Cria o ficheiro de texto
        str = new String(buffer);
        //System.out.println("Msg Recebida:" +str);
    }

    public static void receberImagem(SerialPort port) throws IOException {
        int nBytes;
        System.out.println("NUMBER OF BYTES:");
        Scanner sc = new Scanner(System.in);
        nBytes = sc.nextInt();
        String str;
        System.out.println(nBytes);
        byte[] buffer = new byte[nBytes];
        while (port.bytesAvailable() < nBytes) {
            System.out.println(port.bytesAvailable());
        }
        port.readBytes(buffer, nBytes);
        Files.write(new File("C:\\Users\\tiago\\Desktop\\rece1.gif").toPath(), buffer);//Cria o ficheiro de texto
        str = new String(buffer);
        //System.out.println("Msg Recebida:" +str);
    }
}
