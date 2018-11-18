package Entity;

import javax.persistence.*;
import java.io.Serializable;

import java.util.Date;
import java.util.Set;

@Entity
//@Table(name = "t_student")
//@SequenceGenerator(name = "SEQ_STUDENT", sequenceName = "SEQ_STUDENT")

public class Student implements Serializable {
    private static final long serialVersionUID = 2524659555729848644L;
    private Long id;
    private String name;
    private Date birthday;
    private int sex;
    private String address;
    private Set<Teacher> teacherList;

    private Integer a;


    @Id
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

    @Temporal(TemporalType.DATE)
    @Column(name = "birthday")
    public Date getBirthday() {
        return birthday;
    }

    public void setBirthday(Date birthday) {
        this.birthday = birthday;
    }

    @Column(name = "sex")
    public int getSex() {
        return sex;
    }

    public void setSex(int sex) {
        this.sex = sex;
    }

    @Column(name = "address")
    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    @ManyToMany(cascade = CascadeType.ALL)
    @JoinTable(name = "t_teacher_student",
            joinColumns = @JoinColumn(name = "stu_id",referencedColumnName="id"),
            inverseJoinColumns = @JoinColumn(name = "teacher_id",referencedColumnName ="id"))
    public Set<Teacher> getTeacherList() {
        return teacherList;
    }

    public void setTeacherList(Set<Teacher> teacherList) {
        this.teacherList = teacherList;
    }
}