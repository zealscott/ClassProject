use master
go
drop database pm_dw;


create database pm_dw;
use pm_dw;

create table date
(
    date_key int  primary key,
    detail_date char(10),
    year char(10),
    quarterly char(10),
    month char(10)
)

create table customer
(
    customer_key int primary key,
    customer_name char(10),
    sex char(10),
    age char(10),
    p_ientity bit,
    income int
)

create table location
(
    locate_key int primary key,
    detail_address char(10),
    province char(10),
    city char(10),
    area char(10)
)

create table product
(
    product_key int primary key,
    product_name char(10),
    product_unit_price int,
    product_class char(10)
)

create table sell
(
    product_key int,
    customer_key int,
    locate_key int,
    amount int,
    total_fee int,

    CONSTRAINT FK_product foreign key(product_key) references product(product_key),
    CONSTRAINT FK_customer foreign key(customer_key) references customer(customer_key),
    CONSTRAINT FK_location foreign key(locate_key) references location(locate_key),
    CONSTRAINT PK_sell PRIMARY KEY (product_key,customer_key,locate_key)
)

-- modify table
alter table customer 
add mobile int;

-- insert
insert into date values(1,'5:10',2014,3,9);

insert into customer values(1,'xiaowang','male',50,0,5000,1771742455);
insert into customer values(2,'dage','male',25,0,5000,17717425003);
insert into customer values(3,'xiaoxin','female',50,0,5000,17717425002);
insert into customer values(4,'xiaoming','female',25,0,5000,17717425545);
insert into customer values(5,'duyuntao','male',50,0,5000,177141421);
insert into customer values(6,'qianweining','female',25,0,5000,1734534534);

insert into location values(1,'minhang','shanghai','shanghai','ecnu');
insert into location values(2,'minhang','hangzhou','zhejiang','zheda');
insert into location values(3,'minhang','nanjing','jiangsu','nanda');
insert into location values(4,'minhang','beijing','beijing','qinghau');

insert into product values(1,'food',20,'food');

insert into sell values(1,1,1,30,5000);

-- update
update customer set customer_name='xiaohua' where customer_key=1;
update product set product_name='water' where product_class='food';

-- select
select customer_name,sex,age
from customer
where customer_key=1;

-- delete
-- delete from sell;
-- delete from customer;
-- delete from product;
-- delete from location;
-- delete from date;

-- drop
-- drop table sell;
-- drop table customer;
-- drop table product;
-- drop table location;
-- drop table date;


