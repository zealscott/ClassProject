package TCP;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;


public class TCPServer {
    public static void main(String[] args) {

        try  {
            // create server socket, listening port
            ServerSocket serverSocket = new ServerSocket(8888);
            // call accept() function to get message
            System.out.println("---- TCP server is running, waiting for connecting");
            Socket socket = serverSocket.accept();
            // get input stream
            InputStream in = socket.getInputStream();
            // convert to string input stream
            InputStreamReader inreader = new InputStreamReader(in);
            // add buffer for input stream
            BufferedReader br = new BufferedReader(inreader);
            String info = null;
            while((info = br.readLine())!=null){
                System.out.println("This is TCP server, client says: "+info);

            }
            socket.shutdownInput();

            // get output stream, send to client
            OutputStream outputStream = socket.getOutputStream();
            PrintWriter printWriter = new PrintWriter(outputStream);
            printWriter.write("message from TCP server, welcome! ");
            printWriter.flush();
            socket.shutdownOutput();

            //close resource
            printWriter.close();
            outputStream.close();

            br.close();
            inreader.close();
            in.close();
            socket.close();
            serverSocket.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
