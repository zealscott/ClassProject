drop external table test001_ext_1;
create external table public.test001_ext_1(
    id integer,
    phoneno varchar(20),
    time date,
    name varchar(128)
)
location(
    'gpfdist://10.11.10.9:9058/test001_ext_1.txt'
)
Format 'TEXT' (delimiter as E',' null as '' escape 'OFF');


drop external table test001_ext_2;
create external table public.test001_ext_2(
    log_time timestamp(0),       -- 浏览时间
    cookie_id varchar(256),      -- 浏览的id
    url varchar(1024),             -- 浏览的页面
    ip varchar(64),              -- 用户ip
    refer_url varchar(1024)      -- 域名
)
location(
    'gpfdist://10.11.10.9:9058/data.txt'
)
Format 'TEXT' (delimiter as E',' null as '' escape 'OFF');


select case when cnt>100 then '100+'
		    when cnt>50 then '51-100'
		    when cnt>10 then '11-50'
		    when cnt>5 then '6-10'
		    else '<=5' end tag,
	   count(1) as number
from (
	select cookie_id,
	count(1) cnt
	from log_path_tmp1
	group by 1
)t
group by 1;