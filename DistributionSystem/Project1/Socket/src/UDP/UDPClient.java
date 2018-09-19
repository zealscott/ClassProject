package UDP;

import java.io.IOException;
import java.net.*;


public class UDPClient {
    public static void main(String[] args) {

        try {

            // define IP and port
            InetAddress inetAddress = InetAddress.getByName("localhost");
            int port = 8800;
            byte[] data = "UDP send message, socket connect successful!".getBytes();
            // create DatagramSocket, port:8800
            DatagramPacket packet = new DatagramPacket(data,data.length,inetAddress,port);
            // create data socket
            DatagramSocket socket = new DatagramSocket();
            // send data pocket to server
            socket.send(packet);
            //socket.close();


            // receive server response
            byte[] dataR = new byte[1024];
            DatagramPacket packet1 = new DatagramPacket(dataR,dataR.length);
            socket.receive(packet1);
            // read message
            String info = new String(dataR,0,packet1.getLength());
            System.out.println("This is UDP client,sever says:"+info);

            socket.close();



        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (SocketException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
