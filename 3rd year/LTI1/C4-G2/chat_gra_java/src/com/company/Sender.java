package com.company;

import com.fazecast.jSerialComm.SerialPort;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Scanner;

import static java.lang.Thread.onSpinWait;
import static java.lang.Thread.sleep;

public class Sender extends JFrame {

    private static int portNumber;
    private static boolean receber;
    private JPanel mainPanel;

    public Sender(){
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainPanel.setLayout(null);
        mainPanel.setBackground(new Color(7,94, 84));
        mainPanel.setBounds(0,0,400,60);
        add(mainPanel);

        ImageIcon imageIcon = new ImageIcon(ClassLoader.getSystemResource("com/company/icons/chat.png"));
        Image image1 = imageIcon.getImage().getScaledInstance(60, 60, Image.SCALE_DEFAULT);
        ImageIcon iIcon = new ImageIcon(image1);
        JLabel label = new JLabel(iIcon);
        label.setBounds(40,0,60,60);
        mainPanel.add(label);

        JLabel user = new JLabel("USER");
        user.setFont(new Font("SAN_SERIF", Font.BOLD, 18));
        user.setForeground(Color.WHITE);
        user.setBounds(110, 15, 100, 18);
        mainPanel.add(user);

        JLabel online = new JLabel("Online");
        online.setFont(new Font("SAN_SERIF", Font.PLAIN, 15));
        online.setForeground(Color.WHITE);
        online.setBounds(110, 35, 100, 20);
        mainPanel.add(online);

        getContentPane().setBackground(Color.decode("#FDFDDE"));
        setLayout(null);

    }

    public static void main(String[] args) throws InterruptedException, IOException {

        SerialPort[] ports = SerialPort.getCommPorts();//Array para guardar as portas série disponíveis

        if (ports.length == 0){
            System.out.println("No Ports Available");
            sleep(2000);
            System.exit(0);
        }

        JFrame jFrame2 = new Sender();
        JTextField jTextField2 = new JTextField();
        JButton jButtonCOM = new JButton();
        JTextArea jTextArea2 = new JTextArea();

        jFrame2.setSize(400, 600);
        jFrame2.setLocation(500, 150);

        JLabel portChoice = new JLabel("Choose Port:");
        portChoice.setFont(new Font("SAN_SERIF", Font.BOLD, 18));
        portChoice.setForeground(Color.BLACK);
        portChoice.setBounds(120, 150, 150, 18);
        jFrame2.add(portChoice);

        jTextArea2.setBounds(5,200,370, 250);
        jTextArea2.setBackground(Color.decode("#FDFDDE"));
        jTextArea2.setFont(new Font("SAN_SERIF", Font.PLAIN, 18));
        jTextArea2.setEditable(false);
        int i = 1;
        for (SerialPort port : ports) {     //Seleção da porta série a usar
            //jTextArea2.setText(i++ + ". " +port.getSystemPortName() + "\n");
            jTextArea2.setText(jTextArea2.getText()+"\n"+ i++ + ". " + port.getSystemPortName() + "\n");
        }
        jFrame2.add(jTextArea2);

        jTextField2.setBounds(10,510, 280, 40);
        jTextField2.setFont(new Font("SAN_SERIF", Font.PLAIN, 16));
        jFrame2.add(jTextField2);

        jButtonCOM.setBounds(300, 510, 80, 40);
        jButtonCOM.setBackground(new Color(98,201,182));
        jButtonCOM.setForeground(Color.BLACK);
        jButtonCOM.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                boas(jTextField2, jTextArea2);
                synchronized (jButtonCOM) {
                    jButtonCOM.notify();
                }
            }
        });
        jFrame2.add(jButtonCOM);
        jFrame2.setVisible(true);

        synchronized(jButtonCOM) {
            try {
                jButtonCOM.wait();
            } catch (InterruptedException ex) {
                System.out.println("Interrupted");
            }
        }
        SerialPort port = ports[portNumber - 1];

        if (port.openPort()) {
            System.out.println("Port Opened");
        } else {
            System.out.println("Port Closed");
        }
        port.setComPortParameters(115200, 8, 1, 0);//Configuração da porta série

        jFrame2.setVisible(false);

        JFrame jFrame = new Sender();
        JTextField jTextField = new JTextField();
        JButton jButton = new JButton("SEND");
        JTextArea jTextArea = new JTextArea();

        jFrame.setSize(400, 600);
        jFrame.setLocation(500, 150);

        jTextArea.setBounds(5,65,370, 440);
        jTextArea.setBackground(Color.decode("#FDFDDE"));
        jTextArea.setFont(new Font("SAN_SERIF", Font.PLAIN, 18));
        jTextArea.setEditable(false);
        jTextArea.setLineWrap(true);
        jTextArea.setWrapStyleWord(true);
        jFrame.add(jTextArea);

        jTextField.setBounds(10,510, 280, 40);
        jTextField.setFont(new Font("SAN_SERIF", Font.PLAIN, 16));
        jFrame.add(jTextField);


        jButton.setBounds(300, 510, 80, 40);
        jButton.setBackground(new Color(98,201,182));
        jButton.setForeground(Color.WHITE);
        jButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                try {
                    sendChat(port, jTextField, jTextArea);
                } catch (IOException | InterruptedException ioException) {
                    ioException.printStackTrace();
                }
            }
        });
        jFrame.add(jButton);

        jFrame.setVisible(true);
        while(true) {
            receberChata(port, jTextArea);
        }


    }

    public static void boas(JTextField jTextField2, JTextArea jTextArea2){
        String choose = jTextField2.getText();
        int portNumber2 = Integer.parseInt(choose);
        jTextArea2.setText(jTextArea2.getText()+"\n"+ portNumber2);
        portNumber = portNumber2;
        System.out.println(portNumber);
    }


    //Método para enviar o ficheiro de texto
    public static void sendChat(SerialPort port, JTextField jTextField, JTextArea jTextArea) throws IOException, InterruptedException {
        String out = jTextField.getText();
        System.out.println(out);
        jTextArea.setText(jTextArea.getText()+"S:"+out);
        jTextField.setText("");
        int j, x=0;
        byte[] bytesFromFile;
        bytesFromFile = out.getBytes();
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
            System.out.println("boas");
         }
        }

        public static void receberChata(SerialPort port, JTextArea jTextArea){
            String str;
            byte[] buffer = new byte[30];
            while (port.bytesAvailable() < 30) {
                System.out.println(port.bytesAvailable());
            }
            port.readBytes(buffer, 30);
            str = new String(buffer);
            System.out.println(Arrays.toString(buffer));
            jTextArea.setText(jTextArea.getText() + "\n" + "R:"+str+"\n");

        }

}


