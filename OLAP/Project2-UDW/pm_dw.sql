
-- create database pm_dw;
-- use pm_dw;

-- drop table date,customer,product,sell cascade;

create table date
(
    date_key int  primary key,
    detail_date char(10),
    year char(10),
    quarterly char(10),
    month char(10)
);

create table customer
(
    customer_key int primary key,
    customer_name char(10),
    sex char(10),
    age char(10),
    p_ientity int,
    income int
);
-- modify table
alter table customer 
add mobile int;

create table location
(
    locate_key int primary key,
    detail_address char(10),
    province char(10),
    city char(10),
    area char(10)
);

create table product
(
    product_key int primary key,
    product_name char(10),
    product_unit_price int,
    product_class char(10)
);

create table sell
(
    date_key int,
    product_key int,
    customer_key int,
    locate_key int,
    amount int,
    total_fee int,

    CONSTRAINT FK_date foreign key(date_key) references date(date_key),
    CONSTRAINT FK_product foreign key(product_key) references product(product_key),
    CONSTRAINT FK_customer foreign key(customer_key) references customer(customer_key),
    CONSTRAINT FK_location foreign key(locate_key) references location(locate_key),
    CONSTRAINT PK_sell PRIMARY KEY (date_key,product_key,customer_key,locate_key)
);



-- insert


insert into customer values(1,'xiaowang','male',50,0,5000,17717);
insert into customer values(2,'dage','male',25,0,5000,11742);
insert into customer values(3,'xiaoxin','female',50,0,5000,11202);
insert into customer values(4,'xiaoming','female',25,0,5000,71255);
insert into customer values(5,'duyuntao','male',50,0,5000,1421);
insert into customer values(6,'qwning','female',25,0,5000,13454);

insert into location values(1,'minhang','shanghai','shanghai','ecnu');
insert into location values(2,'minhang','hangzhou','zhejiang','zheda');
insert into location values(3,'minhang','nanjing','jiangsu','nanda');
insert into location values(4,'minhang','beijing','beijing','qinghau');


insert into date values(1,'5:28','2018','4','12');
insert into date values(2,'12:28','2019','4','12');
insert into date values(3,'9:15','2019','3','9');
insert into date values(4,'1:15','2019','1','1');
insert into date values(5,'1:5','2018','1','1');
insert into date values(6,'8:5','2019','2','8');
insert into date values(7,'5:5','2018','2','5');
insert into date values(8,'3:2','2018','1','3');
insert into date values(9,'2:23','2019','1','2');
insert into date values(10,'5:10','2019','3','9');

insert into product values(1,'food1',20,'food');
insert into product values(2,'food2',10,'food');
insert into product values(3,'food3',60,'food');
insert into product values(4,'food4',23,'food');
insert into product values(5,'food5',52,'food');
insert into product values(6,'food6',12,'food');

insert into product values(7,'water1',2,'water');
insert into product values(8,'water2',7,'water');
insert into product values(9,'water3',8,'water');
insert into product values(10,'water4',12,'water');

insert into sell values(1,1,1,1,30,5000);
insert into sell values(1,2,3,4,60,6763);
insert into sell values(4,3,1,4,15,5346);
insert into sell values(5,3,1,3,45,3453);
insert into sell values(6,3,1,2,35,4362);
insert into sell values(8,3,1,3,25,3456);
insert into sell values(9,4,1,2,20,2354);

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


