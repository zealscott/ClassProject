import Entity.Student;
import Entity.Teacher;
import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;

import java.util.HashSet;
import java.util.Set;

public class Main {


    public static void main(final String[] args) throws Exception {
        //创建配置对象(读取配置文档)
        Configuration config = new Configuration().configure();
        //创建会话工厂对象
        SessionFactory sessionFactory = config.buildSessionFactory();
        //会话对象
        Session session = sessionFactory.openSession();
        //这是开启Session的操作
        session.beginTransaction();
        Student s = new Student();
        s.setName("小猪");
        Teacher t = new Teacher();
        t.setName("小李");
        Set<Teacher> t_set = new HashSet<Teacher>();
        t_set.add(t);
        s.setTeacherList(t_set);
        session.save(s);
        //此处才是真正与数据库交互的语句
        session.getTransaction().commit();

        session.close();

    }
}