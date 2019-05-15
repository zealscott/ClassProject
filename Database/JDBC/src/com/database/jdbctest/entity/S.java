package com.database.jdbctest.entity;

public class S {
    private String sno;
    private String sname;
    private String status;
    private String city;

    S(String sno,String sname,String status,String city){
        this.sno = sno;
        this.sname = sname;
        this.status = status;
        this.city = city;
    }


    public String getSno() {
        return sno;
    }

    public void setSno(String sno) {
        this.sno = sno;
    }

    public String getSname() {
        return sname;
    }

    public void setSname(String sname) {
        this.sname = sname;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }
}
