import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import com.fazecast.jSerialComm.SerialPort;

import static java.lang.Thread.sleep;

/* *
 *
 * Inês Barreira Marques, A84923
 * José Pedro Fernandes Peleja, A84436
 * Rui Filipe Ribeiro Freitas, A84121
 * Tiago João Pereira Ferreira, A85392
 *
 * MIETI 2020/2021
 * LTI I - Fase 1
 *
 * Programa java para comunicação via porta série PC-Arduino-Arduino-PC
 *
 *  */

public class Sender {

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
        Scanner sc = new Scanner(System.in);
        int sp = sc.nextInt();
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
        System.out.println("------------ LTI I - FASE 4 ------------");
        System.out.println("----------------------------------------");
        System.out.println(" 1. Send File ");
        System.out.println(" 2. chat ");
        System.out.println(" 3. Exit ");
        System.out.println("----------------------------------------");
        System.out.println("----------------------------------------");
        do {
            System.out.println("Option:");
            op = s.nextInt();
            valido = (op >= 1) && (op <= 5);
            if (!valido) {
                System.out.println("Invalid Option !!!");
            }
        }
        while (!valido);
        switch (op) {
            case 1:
                inicio = System.currentTimeMillis();    //Começa a contar o tempo de envio
                enviarFicheiro(port);              //Método de envio
                fim = System.currentTimeMillis();       //Para de contar o tempo de envio
                tempo = (fim - inicio);                 //Calcula o tempo de envio
                System.out.println(tempo + " ms to send file");
                break;

            case 2://Método para receber
                while (true) {
                    enviaChat(port);
                    recebeChat(port);
                }                   //Fecha a porta série

            case 3:
                System.exit(1);                   //Sai do programa
                break;

        }
    }

    //Método para enviar chat
    public static void enviaChat(SerialPort port) throws IOException, InterruptedException {
        int j, x=0;
        byte[] bytesFromFile;
        System.out.print("EU:");
        Scanner sc = new Scanner(System.in);
        String msg = sc.nextLine();
        bytesFromFile = msg.getBytes();
        float nPac;
        int nPacotes;
        nPac = (float) Math.ceil(((bytesFromFile.length)/30.0));
        nPacotes= (int) nPac;
        for(int i=0;i<nPacotes;i++) {
            byte[] bytesFromF = new byte[31];
            if (i == (nPacotes - 1)) {
                bytesFromF[0] = (byte) 0b0110010;
            } else {
                bytesFromF[0] = (byte) 0b00110001;
            }
            for (j = 0; j < 30; j++) {
                if (j + x == bytesFromFile.length) {
                    break;
                }
                bytesFromF[j + 1] = bytesFromFile[j + x];
            }
            x += 30;
            long inicioo = System.currentTimeMillis();
            port.writeBytes(bytesFromF, 31);//Envia para a porta série o array de bytes
            byte[] bufferR = new byte[2];//Array para guardar os bytes recebidos pela porta série
            while (port.bytesAvailable() < 2000000) {//Espera pelo envio do último byte
                port.readBytes(bufferR, 2);
                if (bufferR[0] == (-114) && bufferR[1] == (-114)) {
                    break;
                }
            }
            long fimm = System.currentTimeMillis();
            long tempoo = (fimm - inicioo);
            System.out.println(tempoo + " ms to send message");
        }
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

    public static void enviarFicheiro(SerialPort port) throws IOException, InterruptedException {
        int j, x=0;
        System.out.println("Escreva o nome do ficheiro:");
        Scanner sc = new Scanner(System.in);
        String fileN = sc.nextLine();
        String n = "C:\\Users\\tiago\\Desktop\\"+fileN;
        byte[] bytesFromFile = Files.readAllBytes(Paths.get(n));
        float nPac;
        int nPacotes;
        nPac = (float) Math.ceil(((bytesFromFile.length)/30.0));
        nPacotes= (int) nPac;
        for(int i=0;i<nPacotes;i++) {
            byte[] bytesFromF = new byte[31];
            if (i == (nPacotes - 1)) {
                bytesFromF[0] = (byte) 0b0110010;
            } else {
                bytesFromF[0] = (byte) 0b00110001;
            }
            for (j = 0; j < 30; j++) {
                if (j + x == bytesFromFile.length) {
                    break;
                }
                bytesFromF[j + 1] = bytesFromFile[j + x];
            }
            x += 30;
            long inicioo = System.currentTimeMillis();
            port.writeBytes(bytesFromF, 31);//Envia para a porta série o array de bytes
            byte[] bufferR = new byte[2];//Array para guardar os bytes recebidos pela porta série
            while (port.bytesAvailable() < 2000000) {//Espera pelo envio do último byte
                port.readBytes(bufferR, 2);
                if (bufferR[0] == (-114) && bufferR[1] == (-114)) {
                    break;
                }
            }
            long fimm = System.currentTimeMillis();
            long tempoo = (fimm - inicioo);
            System.out.println(tempoo + " ms to send message");
        }
    }
}
