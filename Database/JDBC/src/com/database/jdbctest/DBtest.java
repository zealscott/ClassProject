package com.database.jdbctest;


import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;

public class DBtest {
    public static void main(String args[]) {
        Connection c = null;
        Statement stmt = null;
        try {
            Class.forName("org.postgresql.Driver");
            c = DriverManager.getConnection("jdbc:postgresql://localhost:5432/spj", "postgres", "5183");
            c.setAutoCommit(false); // 把自动提交
            System.out.println("Opened database successfully");

            stmt = c.createStatement();
            String sql = "CREATE TABLE STUDENTS " +
                    "(ID TEXT PRIMARY KEY     NOT NULL ," +
                    " NAME            TEXT    NOT NULL, " +
                    " SEX             TEXT    NOT NULL, " +
                    " AGE             TEXT    NOT NULL)";
            stmt.executeUpdate(sql);
            System.out.println("Table created successfully");

            stmt.close();
            c.commit();
            c.close();
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
    }
}

