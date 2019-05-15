package Entity;
import Entity.Student;

import javax.persistence.*;
import java.io.Serializable;
import java.util.Set;

@Entity
//@Table(name = "t_teacher")
//@SequenceGenerator(name = "SEQ_TEACHER", sequenceName = "SEQ_TEACHER")

public class Teacher implements Serializable {
    private static final long serialVersionUID = 2297316923535111793L;
    private Long id;
    private String name;
    private int sex;
    private Set<Student> studentList;

    @Id
//    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "SEQ_TEACHER")
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id", nullable = false, precision = 22, scale = 0)
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    @Column(name = "name")
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Column(name = "sex")
    public int getSex() {
        return sex;
    }

    public void setSex(int sex) {
        this.sex = sex;
    }

    @ManyToMany(mappedBy = "teacherList", cascade = CascadeType.ALL)
    public Set<Student> getStudentList() {
        return studentList;
    }

    public void setStudentList(Set<Student> studentList) {
        this.studentList = studentList;
    }
}