package com.database.jdbctest;

import com.database.jdbctest.entity.S;

import java.sql.*;

public class JDBCOperation {

    /**
     * @method getConn() 获取数据库的连接
     * @return Connection
     */
    public Connection getConn() {
        String driver = "org.postgresql.Driver";
        String url = "jdbc:postgresql://udw.lurymd.m0.service.ucloud.cn:5432/db_dyt";
        String username = "dyt";
        String password = "dyt123";
        Connection conn = null;
        try {
            Class.forName(driver); // classLoader,加载对应驱动
            conn = DriverManager.getConnection(url, username, password);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return conn;
    }

    /**
     * @method insert(Student student) 往表中插入数据
     * @return int 成功插入数据条数
     */
    public int insert(S s) {
        Connection conn = getConn();
        int i = 0;
        String sql = "insert into s (sno,sname,status,city) values(?,?,?,?)";
        PreparedStatement pstmt;
        try {
            pstmt = (PreparedStatement) conn.prepareStatement(sql);
            pstmt.setString(1, s.getSno());
            pstmt.setString(2, s.getSname());
            pstmt.setString(3, s.getStatus());
            pstmt.setString(4, s.getCity());
            i = pstmt.executeUpdate();
            pstmt.close();
            conn.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return i;
    }

    /**
     * @method delete(Student student) 删除表中数据
     * @return int 成功删除表中数据条数
     */
    public int delete(String no) {
        Connection conn = getConn();
        int i = 0;
        String sql = "delete from S where sno=?";
        PreparedStatement pstmt;
        try {
            pstmt = (PreparedStatement) conn.prepareStatement(sql);
            pstmt.setString(1,no);
            i = pstmt.executeUpdate();
            System.out.println("resutl: " + i);
            pstmt.close();
            conn.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return i;
    }

    /**
     * @method update(Student student) 更改表中数据
     * @return int 成功更改表中数据条数
     */
    public int update(S s) {
        Connection conn = getConn();
        int i = 0;
        String sql = "update S set sname= ? where sno=?  ";
        PreparedStatement pstmt;
        try {
            pstmt = (PreparedStatement) conn.prepareStatement(sql);
            pstmt.setString(1,s.getSname());
            pstmt.setString(2,s.getSno());

            i = pstmt.executeUpdate();
            System.out.println("resutl: " + i);
            pstmt.close();
            conn.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return i;
    }

    /**
     * @method Integer getAll() 查询并打印表中数据
     * @return Integer 查询并打印表中数据
     */
    public Integer getAll(String table) {
        Connection conn = getConn();
        String sql = "select * from "+table;
        PreparedStatement pstmt;
        try {
            pstmt = (PreparedStatement)conn.prepareStatement(sql);
            ResultSet rs = pstmt.executeQuery();
            int col = rs.getMetaData().getColumnCount();
            System.out.println("============================");
//            打印每一列
            while (rs.next()) {
                for (int i = 1; i <= col; i++) {
                    System.out.print(rs.getString(i) + "\t");
                    if ((i == 2) && (rs.getString(i).length() < 8)) {
                        System.out.print("\t");
                    }
                }
                System.out.println("");
            }
            System.out.println("============================");
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @method Integer SelectCity(String City) 查询城市
     * @return Integer 查询并打印表中数据
     */
    public void SelectCity(String City){
        Connection conn = getConn();
        String sql = "select * from S where city = ? ";
        PreparedStatement pstmt;
        try {
            pstmt = (PreparedStatement) conn.prepareStatement(sql);
            pstmt.setString(1,City);

            ResultSet rs = pstmt.executeQuery();
            int col = rs.getMetaData().getColumnCount();
            System.out.println("============================");
//            打印每一列
            while (rs.next()) {
                for (int i = 1; i <= col; i++) {
                    System.out.print(rs.getString(i) + "\t");
                    if ((i == 2) && (rs.getString(i).length() < 8)) {
                        System.out.print("\t");
                    }
                }
                System.out.println("");
            }
            pstmt.close();
            conn.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

    }
}
