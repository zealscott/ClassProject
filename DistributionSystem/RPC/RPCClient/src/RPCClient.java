import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.net.Socket;

public class RPCClient {

    public static void main(String args[]){
        // This helloService is a proxy object,it will call remote real object to finish method
        // need helloService interface
        HelloService helloService = getClient(HelloService.class, "127.0.0.1", 50001);
        System.out.println(helloService.hello("duyuntao"));
    }

    @SuppressWarnings("unchecked")
    public static <T> T getClient(Class<T> clazz, String ip, int port){
        return  (T) Proxy.newProxyInstance(RPCClient.class.getClassLoader(), new Class<?>[]{clazz}, new InvocationHandler() {
            // this is a anonymous inner class
                @Override
                public Object invoke(Object arg0, Method method, Object[] arg) throws Throwable {
                    Socket socket = new Socket(ip, port);
                    ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
                    out.writeUTF(method.getName());
                    out.writeObject(method.getParameterTypes());
                    out.writeObject(arg);
                    ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
                    return in.readObject();
            }
        });
    }
}