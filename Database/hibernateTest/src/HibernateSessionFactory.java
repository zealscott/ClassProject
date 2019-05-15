import entity.PlayerEntity;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.boot.MetadataSources;
import org.hibernate.boot.registry.StandardServiceRegistry;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;

import java.util.List;

public class HibernateSessionFactory {

    public static void main(String[] args) {
        //创建配置对象(读取配置文档)
        Configuration config = new Configuration().configure();
        //创建会话工厂对象
        SessionFactory sessionFactory = config.buildSessionFactory();
        //会话对象
        Session session = sessionFactory.openSession();
        //这是开启Session的操作
        session.beginTransaction();

        PlayerEntity player1 = new PlayerEntity();
        player1.setCoin(10);
        player1.setUserName("scott");

        //这正是把数据放入一级缓存session中的操作
        session.save(player1);
        //此处才是真正与数据库交互的语句
        session.getTransaction().commit();

        session.close();
    }

}


