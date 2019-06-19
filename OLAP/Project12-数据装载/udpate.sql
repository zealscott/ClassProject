USE source;  
  
/***     
客户数据的改变如下：  
客户6的街道号改为7777 Ritter Rd。（原来是7070 Ritter Rd）  
客户7的姓名改为Distinguished Agencies。（原来是Distinguished Partners）  
新增第八个客户。  
***/  
UPDATE customer SET customer_street_address = '7777 Ritter Rd.' WHERE customer_number = 6 ;  
UPDATE customer SET customer_name = 'Distinguished Agencies' WHERE customer_number = 7 ;  
INSERT INTO customer  
(customer_name, customer_street_address, customer_zip_code, customer_city, customer_state)  
VALUES  
('Subsidiaries', '10000 Wetline Blvd.', 17055, 'Pittsburgh', 'PA') ;  
  
/***  
产品数据的改变如下：  
产品3的名称改为Flat Panel。（原来是LCD Panel）  
新增第四个产品。  
***/  
UPDATE product SET product_name = 'Flat Panel' WHERE product_code = 3 ;  
INSERT INTO product  
(product_name, product_category)  
VALUES  
('Keyboard', 'Peripheral') ;   
    
/***    
新增订单日期为2016年7月4日的16条订单。    
***/  
SET @start_date := unix_timestamp('2016-07-04');  
SET @end_date := unix_timestamp('2016-07-05');  
DROP TABLE IF EXISTS temp_sales_order_data;  
CREATE TABLE temp_sales_order_data AS SELECT * FROM sales_order WHERE 1=0;   
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (101, 1, 1, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (102, 2, 2, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (103, 3, 3, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (104, 4, 4, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (105, 5, 2, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (106, 6, 2, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (107, 7, 3, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (108, 8, 4, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (109, 1, 1, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (110, 2, 2, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (111, 3, 3, @order_date, @order_date, @amount);  
    
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (112, 4, 4, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (113, 5, 1, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (114, 6, 2, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (115, 7, 3, @order_date, @order_date, @amount);  
  
SET @order_date := from_unixtime(@start_date + rand() * (@end_date - @start_date));  
SET @amount := floor(1000 + rand() * 9000);  
INSERT INTO temp_sales_order_data VALUES (116, 8, 4, @order_date, @order_date, @amount);  
  
INSERT INTO sales_order  
SELECT NULL,customer_number,product_code,order_date,entry_date,order_amount FROM temp_sales_order_data ORDER BY order_date;    
  
COMMIT ;   