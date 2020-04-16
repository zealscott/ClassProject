package test;

import org.apache.zookeeper.*;
import org.apache.zookeeper.data.Stat;
import org.junit.Test;
import org.junit.Before; 
import org.junit.After;

import java.util.List;

/** 
* Simple Tester. 
* 
* @author <Authors name> 
* @since <pre>Nov 11, 2018</pre> 
* @version 1.0 
*/ 
public class SimpleTest {
    private static final String connectString = "localhost:2181";
    private static final int sessionTimeout = 2000;

    private ZooKeeper zkClient = null;
@Before
public void testInit() throws Exception {
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
    String nodeCreated = zkClient.create("/scott", "hellozk".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
}


/** 
* 
* Method: Exist() 
* 
*/ 
@Test
public void testExist() throws Exception {
    Stat stat = zkClient.exists("/scott", false);
    System.out.println(stat==null?"not exist":"exist");
} 

/** 
* 
* Method: getChildren() 
* 
*/ 
@Test
public void testGetChildren() throws Exception {
    List<String> children = zkClient.getChildren("/", true);
    for (String child : children) {
        System.out.println(child);
    }
//    Thread.sleep(Long.MAX_VALUE);
} 

/** 
* 
* Method: getData() 
* 
*/ 
@Test
public void testGetData() throws Exception {
    byte[] data = zkClient.getData("/scott", false, null);
    System.out.println(new String(data));
} 

/** 
* 
* Method: deleteZnode() 
* 
*/ 
@Test
public void testDeleteZnode() throws Exception {
    zkClient.delete("/scott", -1);
} 

/** 
* 
* Method: setData() 
* 
*/ 
@Test
public void testSetData() throws Exception {
    zkClient.setData("/scott", "imissyou angelababy".getBytes(), -1);

    byte[] data = zkClient.getData("/scott", false, null);
    System.out.println(new String(data));
} 


} 
