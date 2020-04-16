import java.util.List;

import org.apache.zookeeper.*;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.data.Stat;


public class Simple {

    private static final String connectString = "localhost:2181";
    private static final int sessionTimeout = 2000;

    private ZooKeeper zkClient = null;

    public void init() throws Exception {
        zkClient = new ZooKeeper(connectString, sessionTimeout, new Watcher() {
            @Override
            public void process(WatchedEvent event) {
                // 收到事件通知后的回调函数（应该是我们自己的事件处理逻辑）
                System.out.println(event.getType() + "---" + event.getPath());
                try {
                    zkClient.getChildren("/", true);
                } catch (Exception e) {
                }
            }
        });

    }

    /**
     * 数据的增删改查
     *
     * @throws InterruptedException
     * @throws KeeperException
     */

    // 创建数据节点到zk中
    public void Create() throws KeeperException, InterruptedException {
        // 参数1：要创建的节点的路径 参数2：节点大数据 参数3：节点的权限 参数4：节点的类型
        String nodeCreated = zkClient.create("/scott", "hellozk".getBytes(), Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
        //上传的数据可以是任何类型，但都要转成byte[]
    }

    //判断znode是否存在

    public void Exist() throws Exception{
        Stat stat = zkClient.exists("/scott", false);
        System.out.println(stat==null?"not exist":"exist");


    }

    // 获取子节点
    public void getChildren() throws Exception {
        List<String> children = zkClient.getChildren("/", true);
        for (String child : children) {
            System.out.println(child);
        }
        Thread.sleep(Long.MAX_VALUE);
    }

    //获取znode的数据
    public void getData() throws Exception {

        byte[] data = zkClient.getData("/scott", false, null);
        System.out.println(new String(data));

    }

    //删除znode
    public void deleteZnode() throws Exception {

        //参数2：指定要删除的版本，-1表示删除所有版本
        zkClient.delete("/eclipse", -1);


    }
    //删除znode
    public void setData() throws Exception {

        zkClient.setData("/scott", "imissyou angelababy".getBytes(), -1);

        byte[] data = zkClient.getData("/scott", false, null);
        System.out.println(new String(data));

    }
}
