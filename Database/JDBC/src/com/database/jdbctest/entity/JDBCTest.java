package com.database.jdbctest.entity;

import com.database.jdbctest.JDBCOperation;

public class JDBCTest {
    public static void main(String args[]) {
        JDBCOperation op = new JDBCOperation();
//        op.getAll();
//        op.insert(new S("S6","徐瑞", "20", "上海"));
//        op.insert(new S("S7","胡", "30", "重庆"));
//        op.getAll();
//        op.update(new S("S7","耀艺", "", "上海"));
//        op.delete("S6");
//        op.getAll();

        op.SelectCity("上海");
    }

}