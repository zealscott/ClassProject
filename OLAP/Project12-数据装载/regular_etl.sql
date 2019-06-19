-- 设置变量以支持事务  
set hive.support.concurrency=true;  
set hive.exec.dynamic.partition.mode=nonstrict;  
set hive.txn.manager=org.apache.hadoop.hive.ql.lockmgr.DbTxnManager;  
set hive.compactor.initiator.on=true;  
set hive.compactor.worker.threads=1;  
  
USE dw;  
    
-- 设置SCD的生效时间和过期时间  
SET hivevar:cur_date = CURRENT_DATE();  
SET hivevar:pre_date = DATE_ADD(${hivevar:cur_date},-1);  
SET hivevar:max_date = CAST('2200-01-01' AS DATE);  
    
-- 设置CDC的上限时间  
INSERT OVERWRITE TABLE rds.cdc_time SELECT last_load, ${hivevar:cur_date} FROM rds.cdc_time;  
  
-- 装载customer维度  
-- 设置已删除记录和customer_street_addresses列上SCD2的过期    
UPDATE customer_dim   
   SET expiry_date = ${hivevar:pre_date}    
 WHERE customer_dim.customer_sk IN    
(SELECT a.customer_sk   
   FROM (SELECT customer_sk,customer_number,customer_street_address   
           FROM customer_dim WHERE expiry_date = ${hivevar:max_date}) a LEFT JOIN   
                rds.customer b ON a.customer_number = b.customer_number   
          WHERE b.customer_number IS NULL OR a.customer_street_address <> b.customer_street_address);   
  
-- 处理customer_street_addresses列上SCD2的新增行    
INSERT INTO customer_dim  
SELECT  
    ROW_NUMBER() OVER (ORDER BY t1.customer_number) + t2.sk_max,  
    t1.customer_number,  
    t1.customer_name,  
    t1.customer_street_address,  
    t1.customer_zip_code,  
    t1.customer_city,  
    t1.customer_state,  
    t1.version,    
    t1.effective_date,    
    t1.expiry_date    
FROM    
(    
SELECT    
    t2.customer_number customer_number,  
    t2.customer_name customer_name,  
    t2.customer_street_address customer_street_address,  
    t2.customer_zip_code,  
    t2.customer_city,  
    t2.customer_state,  
    t1.version + 1 version,  
    ${hivevar:pre_date} effective_date,    
    ${hivevar:max_date} expiry_date    
 FROM customer_dim t1   
INNER JOIN rds.customer t2    
   ON t1.customer_number = t2.customer_number     
  AND t1.expiry_date = ${hivevar:pre_date}    
 LEFT JOIN customer_dim t3   
   ON t1.customer_number = t3.customer_number   
  AND t3.expiry_date = ${hivevar:max_date}    
WHERE t1.customer_street_address <> t2.customer_street_address AND t3.customer_sk IS NULL) t1    
CROSS JOIN    
(SELECT COALESCE(MAX(customer_sk),0) sk_max FROM customer_dim) t2;  
  
-- 处理customer_name列上的SCD1  
-- 因为hive里update的set子句还不支持子查询，所以这里使用了一个临时表存储需要更新的记录，用先delete再insert代替update，为简单起见也不考虑并发问题（数据仓库应用的并发操作基本都是只读的，很少并发写，所以并发导致的问题并不像OLTP那样严重）。  
-- 因为SCD1本身就不保存历史数据，所以这里更新维度表里的所有customer_name改变的记录，而不是仅仅更新当前版本的记录  
DROP TABLE IF EXISTS tmp;  
CREATE TABLE tmp AS  
SELECT  
    a.customer_sk,  
    a.customer_number,  
    b.customer_name,  
    a.customer_street_address,  
    a.customer_zip_code,  
    a.customer_city,  
    a.customer_state,  
    a.version,  
    a.effective_date,  
    a.expiry_date  
  FROM customer_dim a, rds.customer b    
 WHERE a.customer_number = b.customer_number AND (a.customer_name <> b.customer_name);    
DELETE FROM customer_dim WHERE customer_dim.customer_sk IN (SELECT customer_sk FROM tmp);    
INSERT INTO customer_dim SELECT * FROM tmp;  
  
-- 处理新增的customer记录   
INSERT INTO customer_dim  
SELECT  
    ROW_NUMBER() OVER (ORDER BY t1.customer_number) + t2.sk_max,  
    t1.customer_number,  
    t1.customer_name,  
    t1.customer_street_address,  
    t1.customer_zip_code,  
    t1.customer_city,  
    t1.customer_state,   
    1,  
    ${hivevar:pre_date},  
    ${hivevar:max_date}  
FROM    
(    
SELECT t1.* FROM rds.customer t1 LEFT JOIN customer_dim t2 ON t1.customer_number = t2.customer_number    
 WHERE t2.customer_sk IS NULL) t1    
CROSS JOIN    
(SELECT COALESCE(MAX(customer_sk),0) sk_max FROM customer_dim) t2;  
  
-- 装载product维度  
-- 设置已删除记录和product_name、product_category列上SCD2的过期  
UPDATE product_dim  
   SET expiry_date = ${hivevar:pre_date}    
 WHERE product_dim.product_sk IN    
(SELECT a.product_sk   
   FROM (SELECT product_sk,product_code,product_name,product_category   
           FROM product_dim WHERE expiry_date = ${hivevar:max_date}) a LEFT JOIN   
                rds.product b ON a.product_code = b.product_code   
          WHERE b.product_code IS NULL OR (a.product_name <> b.product_name OR a.product_category <> b.product_category));  
  
-- 处理product_name、product_category列上SCD2的新增行    
INSERT INTO product_dim  
SELECT  
    ROW_NUMBER() OVER (ORDER BY t1.product_code) + t2.sk_max,  
    t1.product_code,  
    t1.product_name,  
    t1.product_category,  
    t1.version,  
    t1.effective_date,  
    t1.expiry_date  
FROM    
(    
SELECT    
    t2.product_code product_code,  
    t2.product_name product_name,  
    t2.product_category product_category,      
    t1.version + 1 version,  
    ${hivevar:pre_date} effective_date,    
    ${hivevar:max_date} expiry_date    
 FROM product_dim t1   
INNER JOIN rds.product t2    
   ON t1.product_code = t2.product_code    
  AND t1.expiry_date = ${hivevar:pre_date}    
 LEFT JOIN product_dim t3   
   ON t1.product_code = t3.product_code   
  AND t3.expiry_date = ${hivevar:max_date}    
WHERE (t1.product_name <> t2.product_name OR t1.product_category <> t2.product_category) AND t3.product_sk IS NULL) t1    
CROSS JOIN    
(SELECT COALESCE(MAX(product_sk),0) sk_max FROM product_dim) t2;  
  
-- 处理新增的product记录  
INSERT INTO product_dim  
SELECT  
    ROW_NUMBER() OVER (ORDER BY t1.product_code) + t2.sk_max,  
    t1.product_code,  
    t1.product_name,  
    t1.product_category,  
    1,  
    ${hivevar:pre_date},  
    ${hivevar:max_date}  
FROM    
(    
SELECT t1.* FROM rds.product t1 LEFT JOIN product_dim t2 ON t1.product_code = t2.product_code    
 WHERE t2.product_sk IS NULL) t1    
CROSS JOIN    
(SELECT COALESCE(MAX(product_sk),0) sk_max FROM product_dim) t2;  
  
-- 装载order维度  
INSERT INTO order_dim  
SELECT  
    ROW_NUMBER() OVER (ORDER BY t1.order_number) + t2.sk_max,  
    t1.order_number,  
    t1.version,  
    t1.effective_date,  
    t1.expiry_date  
  FROM  
(  
SELECT  
    order_number order_number,  
    1 version,  
    order_date effective_date,  
    '2200-01-01' expiry_date  
  FROM rds.sales_order, rds.cdc_time   
 WHERE entry_date >= last_load AND entry_date < current_load ) t1  
CROSS JOIN    
(SELECT COALESCE(MAX(order_sk),0) sk_max FROM order_dim) t2;  
  
-- 装载销售订单事实表  
INSERT INTO sales_order_fact  
SELECT  
    order_sk,  
    customer_sk,  
    product_sk,  
    date_sk,  
    order_amount  
  FROM  
    rds.sales_order a,  
    order_dim b,  
    customer_dim c,  
    product_dim d,  
    date_dim e,  
    rds.cdc_time f  
 WHERE  
    a.order_number = b.order_number  
AND a.customer_number = c.customer_number  
AND a.order_date >= c.effective_date  
AND a.order_date < c.expiry_date  
AND a.product_code = d.product_code  
AND a.order_date >= d.effective_date  
AND a.order_date < d.expiry_date  
AND to_date(a.order_date) = e.`date`
AND a.entry_date >= f.last_load AND a.entry_date < f.current_load ;  
  
-- 更新时间戳表的last_load字段  
INSERT OVERWRITE TABLE rds.cdc_time SELECT current_load, current_load FROM rds.cdc_time;  