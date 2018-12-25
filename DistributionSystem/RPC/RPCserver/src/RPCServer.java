import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class RPCServer {

    private ExecutorService threadPool;
    private static final int DEFAULT_THREAD_NUM = 10;

    public RPCServer(){
        threadPool = Executors.newFixedThreadPool(DEFAULT_THREAD_NUM);
    }

    public void register(Object service, int port){
        try {
            System.out.println("server starts...");
            ServerSocket server = new ServerSocket(port);
            Socket socket = null;
            while((socket = server.accept()) != null){
                System.out.println("client connected...");
                // thread execute new service
                threadPool.execute(new Processor(socket, service));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // implement Runnable interface(thread)
    class Processor implements Runnable{
        Socket socket;
        Object service;

        public Processor(Socket socket, Object service){
            this.socket = socket;
            this.service = service;
        }
        public void process(){

        }
        @Override
        public void run() {
            try {
                //
                ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
                String methodName = in.readUTF();
                Class<?>[] parameterTypes = (Class<?>[]) in.readObject();
                Object[] parameters = (Object[]) in.readObject();
                Method method = service.getClass().getMethod(methodName, parameterTypes);
                try {
                    Object result = method.invoke(service, parameters);
                    ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
                    out.writeObject(result);
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                } catch (IllegalArgumentException e) {
                    e.printStackTrace();
                } catch (InvocationTargetException e) {
                    e.printStackTrace();
                }
            } catch (IOException e) {
                e.printStackTrace();
            } catch (NoSuchMethodException e) {
                e.printStackTrace();
            } catch (SecurityException e) {
                e.printStackTrace();
            } catch (ClassNotFoundException e1) {
                e1.printStackTrace();
            }
        }
    }
}