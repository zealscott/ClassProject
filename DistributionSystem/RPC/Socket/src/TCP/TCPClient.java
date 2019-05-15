package TCP;

import java.io.*;
import java.net.Socket;

public class TCPClient {
    public static void main(String[] args) {
        try {
            // create socket to connect, assign host and port
            Socket socket = new Socket("127.0.0.1",8888);
            // create output stream, send message to server
            OutputStream outputStream = socket.getOutputStream();
            // convert to print stream
            PrintWriter pw = new PrintWriter(outputStream);
            // send message
            pw.write("message from TCP client");
            pw.flush();
            socket.shutdownOutput();

            // get server stream
            InputStream inputStream = socket.getInputStream();
            // add buffer for input stream
            BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));
            String info = null;
            while((info = br.readLine())!=null){
                System.out.println("This is TCP client, server says: "+info);

            }
            socket.shutdownInput();

            // close resource
            br.close();
            inputStream.close();
            pw.close();
            outputStream.close();
            socket.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
