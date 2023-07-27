import com.fazecast.jSerialComm.SerialPort;

import javax.swing.*;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Scanner;

import static java.lang.Thread.sleep;

/* *
 *
 * Inês Barreira Marques,       A84923
 * José Pedro Fernandes Peleja, A84436
 * Rui Filipe Ribeiro Freitas,  A84121
 * Tiago João Pereira Ferreira, A85392
 *
 * MIETI 2020/2021
 * LTI I - Fase 1
 *
 * Programa java para comunicação via porta série PC-Arduino-Arduino-PC
 *
 *  */

public class SerialCommunication {
    public static void main(String[] args) throws IOException, InterruptedException {

        long inicio;
        long fim;
        long tempo;
        int op;
        boolean valido;
        Scanner sc = new Scanner(System.in);

        SerialPort[] ports = SerialPort.getCommPorts();     //Array para guardar as portas série disponíveis
        if (ports.length == 0) {
            System.out.println("No Ports Available");
            sleep(2000);
            System.exit(0);
        }

        System.out.println("Choose Port:");
        int i = 1;
        for (SerialPort port : ports) {     //Seleção da porta série a usar
            System.out.println(i++ + ". " + port.getSystemPortName());
        }
        int sp = sc.nextInt();
        SerialPort port = ports[sp - 1];
        //SerialPort port2 = ports[sp];
        //port2.openPort();

        if (port.openPort()) {
            System.out.println("Port Opened");
        } else {
            System.out.println("Port Closed");

        }
        port.setComPortParameters(9600, 8, 1, 0);   //Configuração da porta série

        //Menu principal
        Scanner s = new Scanner(System.in);
        System.out.println("\n----------------------------------------");
        System.out.println("------------ LTI I - FASE 1 ------------");
        System.out.println("----------------------------------------");
        System.out.println(" 1. Send Text File ");
        System.out.println(" 2. Receive Text File ");
        System.out.println(" 3. Send Image File ");
        System.out.println(" 4. Receive Image File ");
        System.out.println(" 5. Exit ");
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
                enviarFicheiroTexto(port);              //Método de envio
                fim = System.currentTimeMillis();       //Para de contar o tempo de envio
                tempo = (fim - inicio);                 //Calcula o tempo de envio
                System.out.println(tempo + " ms to send file");
                break;

            case 2:
                receberFicheiroTexto(port);             //Método para receber
                port.closePort();                       //Fecha a porta série
                break;

            case 3:
                inicio = System.currentTimeMillis();    //Começa a contar o tempo de envio
                enviarImagem(port);                     //Método de envio
                fim = System.currentTimeMillis();       //Para de contar o tempo de envio
                tempo = (fim - inicio);                 //Calcula o tempo de envio
                System.out.println(tempo + "ms to send file");
                break;

            case 4:
                receberImagem(port);                    //Método para receber
                port.closePort();                       //Fecha a porta série
                break;

            case 5:
                port.closePort();
                System.exit(1);                   //Sai do programa
                break;
        }
    }

    //Método para enviar o ficheiro de texto
    public static void enviarFicheiroTexto(SerialPort port) throws IOException {
        byte[] bytesFromFile = Files.readAllBytes(Paths.get("C:\\Users\\rui_f\\Desktop\\Ficheiros_LTI\\enviar\\Envio.txt"));//Converte o ficheiro de texto para um array de bytes
        port.writeBytes(bytesFromFile, 6132);  //Envia para a porta série o array de bytes
    }

    //Método para receber o ficheiro de texto
    public static void receberFicheiroTexto(SerialPort port) throws IOException {
        byte[] buffer = new byte[7285];            //Array para guardar os bytes recebidos pela porta série
        while (port.bytesAvailable() < 7285) {//Espera pelo envio do último byte
            //System.out.println(port.bytesAvailable());
        }
        port.readBytes(buffer, 7285);    //lê os bytes recebidos pela porta série
        Files.write(new File("C:\\Users\\rui_f\\Desktop\\Ficheiros_LTI\\receber\\Recebido.txt").toPath(), buffer);  //Cria o ficheiro de texto
    }

    //Método para enviar a imagem
    public static void enviarImagem(SerialPort port) throws IOException {
        byte[] bytesFromFile = Files.readAllBytes(Paths.get("C:\\Users\\rui_f\\Desktop\\Ficheiros_LTI\\enviar\\Envio.png"));//Converte a imagem para um array de bytes
        port.writeBytes(bytesFromFile, 45394);  //Envia para a porta série o array de bytes
    }

    //Método para receber a imagem
    public static void receberImagem(SerialPort port) throws IOException {
        byte[] buffer = new byte[27652];//Array para guardar os bytes recebidos pela porta série
        while (port.bytesAvailable() < 27652) {//Espera pelo envio do último byte
            //System.out.println(port.bytesAvailable());
        }
        port.readBytes(buffer, 27652);//lê os bytes recebidos pela porta série
        Files.write(new File("C:\\Users\\rui_f\\Desktop\\Ficheiros_LTI\\receber\\Recebido.png").toPath(), buffer);  //Cria a imagem
    }
}