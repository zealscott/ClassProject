package UDP;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class UDPServer {
    public static void main(String[] args) {

        try  {
            // create DatagramSocket, port:8800
            DatagramSocket socket = new DatagramSocket(8800);
            // byte array to receive client message
            byte[] data = new byte[1024];
            DatagramPacket packet = new DatagramPacket(data,data.length);
            // receive data package
            socket.receive(packet);
            // read data
            String info = new String(data,0,packet.getLength());
            System.out.println("I am Server,Client say:"+info);


            // define port and ip
            InetAddress address = packet.getAddress();
            int port = packet.getPort();
            byte[] data2 = "This is UTP server, welcome ".getBytes();
            DatagramPacket packet1 = new DatagramPacket(data2,data2.length,address,port);
            socket.send(packet1);

            socket.close();

        } catch (SocketException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}