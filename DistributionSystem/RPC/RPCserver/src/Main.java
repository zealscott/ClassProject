public class Main {

    public static void main(String args[]){
        HelloService helloService = new HelloServiceImpl();

        RPCServer server = new RPCServer();
        server.register(helloService,50001);
    }
}
