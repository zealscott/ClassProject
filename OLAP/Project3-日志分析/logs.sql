drop table if exists log_path;

create table log_path(
    log_time timestamp(0),       -- 浏览时间
    cookie_id varchar(256),      -- 浏览的id
    url varchar(1024),             -- 浏览的页面
    ip varchar(64),              -- 用户ip
    refer_url varchar(1024)      -- 域名
)distributed by(cookie_id);

insert into log_path select * from test001_ext_2;



drop table if exists log_pv_uv_result;

create table log_pv_uv_result(
    log_time varchar(1024),       
    pv integer,     
    uv integer   
)distributed by(log_time);

insert into log_pv_uv_result
select to_char(log_time,'yyyy-mm-dd HH24:mi:00'),
	   COUNT(1) pv,
	   COUNT(distinct cookie_id) uv
from log_path
group by 1
order by 1;

drop table if exists log_path_tmp1;
create table log_path_tmp1 as
(select
    log_time,
    cookie_id,
    substring(url,E'\\w+://([\w.]+)') AS host,
    split_part(url,'?',1) AS url,
    substring(url,E'member[_]?[i|I]d=(\\w+)') AS member_id,
    regexp_split_to_array(split_part(url,'?',2),'&') AS paras,
    ip,
    refer_url
from log_path)
distributed by (cookie_id);