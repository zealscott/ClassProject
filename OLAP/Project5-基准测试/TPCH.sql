create table PART(
	P_PARTKEY	BIGINT,
	P_NAME		VARCHAR(55),
	P_MFGR		CHAR(25),
	P_BRAND		CHAR(10),
	P_TYPE		VARCHAR(25),
	P_SIZE		INTEGER,
	P_CONTAINER	CHAR(10),
	P_RETAILPRICE	DECIMAL,
	P_COMMENT	VARCHAR(23)
) DISTRIBUTED BY (P_PARTKEY);

create table region(
	R_REGIONKEY		BIGINT,
	R_NAME		CHAR(25),
	R_COMMENT	VARCHAR(152)
) DISTRIBUTED BY (R_REGIONKEY);

create table NATION(
	N_NATIONKEY		BIGINT,
	N_NAME		CHAR(25),
	N_REGIONKEY		BIGINT NOT NULL,
	N_COMMENT	VARCHAR(152)
) DISTRIBUTED BY (N_NATIONKEY);

CREATE TABLE SUPPLIER (
                S_SUPPKEY               BIGINT,
                S_NAME                  CHAR(25),
                S_ADDRESS               VARCHAR(40),
                S_NATIONKEY             BIGINT NOT NULL, -- references N_NATIONKEY
                S_PHONE                 CHAR(15),
                S_ACCTBAL               DECIMAL,
                S_COMMENT               VARCHAR(101)
        ) DISTRIBUTED BY (s_suppkey);

CREATE TABLE CUSTOMER (
                C_CUSTKEY               BIGINT,
                C_NAME                  VARCHAR(25),
                C_ADDRESS               VARCHAR(40),
                C_NATIONKEY             BIGINT NOT NULL, -- references N_NATIONKEY
                C_PHONE                 CHAR(15),
                C_ACCTBAL               DECIMAL,
                C_MKTSEGMENT    CHAR(10),
                C_COMMENT               VARCHAR(117)
        ) DISTRIBUTED BY (c_custkey);

CREATE TABLE PARTSUPP (
                PS_PARTKEY              BIGINT NOT NULL, -- references P_PARTKEY
                PS_SUPPKEY              BIGINT NOT NULL, -- references S_SUPPKEY
                PS_AVAILQTY             INTEGER,
                PS_SUPPLYCOST   DECIMAL,
                PS_COMMENT              VARCHAR(199)
        ) DISTRIBUTED BY (ps_partkey);

CREATE TABLE ORDERS (
                O_ORDERKEY              BIGINT,
                O_CUSTKEY               BIGINT NOT NULL, -- references C_CUSTKEY
                O_ORDERSTATUS   CHAR(1),
                O_TOTALPRICE    DECIMAL,
                O_ORDERDATE             DATE,
                O_ORDERPRIORITY CHAR(15),
                O_CLERK                 CHAR(15),
                O_SHIPPRIORITY  INTEGER,
                O_COMMENT               VARCHAR(79)
        ) DISTRIBUTED BY (o_orderkey);

CREATE TABLE LINEITEM (
                L_ORDERKEY              BIGINT NOT NULL, 
                L_PARTKEY               BIGINT NOT NULL, 
	  L_SUPPKEY               BIGINT NOT NULL, 
                L_LINENUMBER    INTEGER,
                L_QUANTITY              DECIMAL,
                L_EXTENDEDPRICE DECIMAL,
                L_DISCOUNT              DECIMAL,
                L_TAX                   DECIMAL,
                L_RETURNFLAG    CHAR(1),
                L_LINESTATUS    CHAR(1),
                L_SHIPDATE              DATE,
                L_COMMITDATE    DATE,
                L_RECEIPTDATE   DATE,
                L_SHIPINSTRUCT  CHAR(25),
                L_SHIPMODE              CHAR(10),
                L_COMMENT               VARCHAR(44)
        ) DISTRIBUTED BY (l_orderkey);
