


-- 事实表
create table public.member_fatdt0
(
    member_id varchar(64),         -- 会员ID
    phoneno varchar(20),           -- 电话号码
    dw_beg_date date,              -- 生效日期
    dw_end_date date,              -- 失效日期
    dtype char(1),                 -- 类型（历史数据，当前数据）
    dw_status char(1),             -- 数据操作类型（I，D，U）
    dw_ins_date date               -- 数据仓库插入日期
)with(appendonly=true,compresslevel=5)   -- 压缩级别
distributed by (member_id)
PARTITION BY RANGE (dw_end_date)
(
    PARTITION p20111201 START (date '2011-12-01') INCLUSIVE,
    PARTITION p20111202 START (date '2011-12-02') INCLUSIVE,
    PARTITION p20111203 START (date '2011-12-03') INCLUSIVE,
    PARTITION p20111204 START (date '2011-12-04') INCLUSIVE,
    PARTITION p20111205 START (date '2011-12-05') INCLUSIVE,
    PARTITION p20111206 START (date '2011-12-06') INCLUSIVE,
    PARTITION p20111207 START (date '2011-12-07') INCLUSIVE,
    PARTITION p20111231 START (date '2011-12-31') INCLUSIVE
    END (date '3001-01-01') EXCLUSIVE
);

-- 增量表
create table public.member_delta
(
    member_id varchar(64),
    phoneno varchar(20),
    action char(1),                      -- 数据操作类型（I，D，U）
    dw_ins_date date                     -- 类型（新增，删除，更新）
)with(appendonly=true,compresslevel=5)   -- 压缩级别
distributed by (member_id)

-- 临时表
create table public.member_tmp0
(
    member_id varchar(64),
    phoneno varchar(20),
    dw_beg_date date,
    dw_end_date date,
    dtype char(1),
    dw_status char(1),
    dw_ins_date date
)with(appendonly=true,compresslevel=5)   -- 压缩级别
distributed by (member_id)
PARTITION BY LIST (dtype)
(
    PARTITION PHIS VALUES ('H'),      -- 表示历史信息
    PARTITION PCUR VALUES ('C'),      -- 表示当前信息
    DEFAULT PARTITION other
);


-- 临时表1
create table public.member_tmp1
(
    member_id varchar(64),
    phoneno varchar(20),
    dw_beg_date date,
    dw_end_date date,
    dtype char(1),
    dw_status char(1),
    dw_ins_date date
)with(appendonly=true,compresslevel=5)   -- 压缩级别
distributed by (member_id)

-- -- 插入数据
Insert into member_delta values('mem006','1310000006','I','2011-12-03');
Insert into member_delta values('mem002','1310000002','D','2011-12-03');
Insert into member_delta values('mem003','1310000003','U','2011-12-03');

Insert into member_fatdt0 values('mem001','1310000001','2011-12-01','3000-12-31','C','I','2011-12-02');
Insert into member_fatdt0 values('mem002','1310000002','2011-12-01','3000-12-31','C','I','2011-12-02');
Insert into member_fatdt0 values('mem003','1310000003','2011-12-01','3000-12-31','C','I','2011-12-02');
Insert into member_fatdt0 values('mem004','1310000004','2011-12-01','3000-12-31','C','I','2011-12-02');
Insert into member_fatdt0 values('mem004','1310000004','2011-12-01','3000-12-31','C','I','2011-12-02');
Insert into member_fatdt0 values('mem005','1310000005','2011-12-01','3000-12-31','C','I','2011-12-02');


-- -- 数据刷新
-- -- 将member_fatdt0表与member_delta左外连接，相关联的历史数据插入到member_tmp0历史分区，反之插入到member_tmp0的当前分区
-- -- 这里主要处理update和delete操作，若能进行左连接，说明数据有更新，插入历史分区

truncate table public.member_tmp0;
-- 清理临时表
insert into public.member_tmp0
(
    member_id,
    phoneno,
    dw_beg_date,
    dw_end_date,
    dtype,
    dw_status,
    dw_ins_date
)
select a.member_id,a.phoneno,a.dw_beg_date,
        case when b.member_id is null then a.dw_end_date
        else date '2011-12-02'
        end as dw_end_date,
        case when b.member_id is null then 'C'
        else 'H'
        end as dtype,
        case when b.member_id is null then a.dw_status
        else b.action
        end as dw_status,
        date '2011-12-03'
from public.member_fatdt0 a
left join public.member_delta b 
on a.member_id = b.member_id
and b.action in('D','U')
where a.dw_beg_date <= cast('2011-12-02' as date) -1
and a.dw_end_date > cast('2011-12-02' as date)-1;

-- -- 将member_delta当前数据（更新，插入的新数据）插入到member_tmp0当前分区，end时间为无穷。
insert into public.member_tmp0
(
    member_id,
    phoneno,
    dw_beg_date,
    dw_end_date,
    dtype,
    dw_status,
    dw_ins_date
)
select member_id,phoneno,
    cast('2011-12-02' as date),
    cast('3000-12-31' as date),
    'C',
    action,
    cast('2011-12-03' as date)
from public.member_delta
where action in ('I','U');

-- -- 将member_tmp0历史数据与member_fatdt0相应分区交换。
truncate table public.member_tmp1;
alter table public.member_tmp0 exchange partition for ('H') with table public.member_tmp1;

alter table public.member_fatdt0 exchange partition for('2011-12-02') with table public.member_tmp1;

-- -- 将member_tmp0当前数据与member_fatdt0相应分区交换
alter table public.member_tmp0 exchange partition for('C') with table public.member_tmp1;
alter table public.member_fatdt0 exchange partition for('3000-12-31') with table public.member_tmp1;