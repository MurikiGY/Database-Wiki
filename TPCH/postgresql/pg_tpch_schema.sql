--
-- PostgreSQL database dump
--

-- Dumped from database version 15.4
-- Dumped by pg_dump version 15.4

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: customer; Type: TABLE; Schema: public; Owner: tpch
--

CREATE TABLE public.CUSTOMER ( 
	C_CUSTKEY     INTEGER NOT NULL,
	C_NAME        VARCHAR(25) NOT NULL,
	C_ADDRESS     VARCHAR(40) NOT NULL,
	C_NATIONKEY   INTEGER NOT NULL,
	C_PHONE       CHAR(15) NOT NULL,
	C_ACCTBAL     DECIMAL(15,2)   NOT NULL,
	C_MKTSEGMENT  CHAR(10) NOT NULL,
	C_COMMENT     VARCHAR(117) NOT NULL);


ALTER TABLE public.customer OWNER TO tpch;

--
-- Name: lineitem; Type: TABLE; Schema: public; Owner: tpch
--

CREATE TABLE public.LINEITEM ( 
	L_ORDERKEY    INTEGER NOT NULL,
	L_PARTKEY     INTEGER NOT NULL,
	L_SUPPKEY     INTEGER NOT NULL,
	L_LINENUMBER  INTEGER NOT NULL,
	L_QUANTITY    DECIMAL(15,2) NOT NULL,
	L_EXTENDEDPRICE  DECIMAL(15,2) NOT NULL,
	L_DISCOUNT    DECIMAL(15,2) NOT NULL,
	L_TAX         DECIMAL(15,2) NOT NULL,
	L_RETURNFLAG  CHAR(1) NOT NULL,
	L_LINESTATUS  CHAR(1) NOT NULL,
	L_SHIPDATE    DATE NOT NULL,
	L_COMMITDATE  DATE NOT NULL,
	L_RECEIPTDATE DATE NOT NULL,
	L_SHIPINSTRUCT CHAR(25) NOT NULL,
	L_SHIPMODE     CHAR(10) NOT NULL,
	L_COMMENT      VARCHAR(44) NOT NULL);


ALTER TABLE public.lineitem OWNER TO tpch;

--
-- Name: nation; Type: TABLE; Schema: public; Owner: tpch
--

CREATE TABLE public.NATION  ( 
	N_NATIONKEY  INTEGER NOT NULL,
	N_NAME       CHAR(25) NOT NULL,
	N_REGIONKEY  INTEGER NOT NULL,
	N_COMMENT    VARCHAR(152));


ALTER TABLE public.nation OWNER TO tpch;

--
-- Name: orders; Type: TABLE; Schema: public; Owner: tpch
--

CREATE TABLE public.ORDERS  ( 
	O_ORDERKEY       INTEGER NOT NULL,
	O_CUSTKEY        INTEGER NOT NULL,
	O_ORDERSTATUS    CHAR(1) NOT NULL,
	O_TOTALPRICE     DECIMAL(15,2) NOT NULL,
	O_ORDERDATE      DATE NOT NULL,
	O_ORDERPRIORITY  CHAR(15) NOT NULL,
	O_CLERK          CHAR(15) NOT NULL,
	O_SHIPPRIORITY   INTEGER NOT NULL,
	O_COMMENT        VARCHAR(79) NOT NULL);

ALTER TABLE public.orders OWNER TO tpch;

--
-- Name: part; Type: TABLE; Schema: public; Owner: tpch
--

CREATE TABLE public.PART  ( 
	P_PARTKEY     INTEGER NOT NULL,
	P_NAME        VARCHAR(55) NOT NULL,
	P_MFGR        CHAR(25) NOT NULL,
	P_BRAND       CHAR(10) NOT NULL,
	P_TYPE        VARCHAR(25) NOT NULL,
	P_SIZE        INTEGER NOT NULL,
	P_CONTAINER   CHAR(10) NOT NULL,
	P_RETAILPRICE DECIMAL(15,2) NOT NULL,
	P_COMMENT     VARCHAR(23) NOT NULL );


ALTER TABLE public.part OWNER TO tpch;

--
-- Name: partsupp; Type: TABLE; Schema: public; Owner: tpch
--

CREATE TABLE public.PARTSUPP ( 
	PS_PARTKEY     INTEGER NOT NULL,
	PS_SUPPKEY     INTEGER NOT NULL,
	PS_AVAILQTY    INTEGER NOT NULL,
	PS_SUPPLYCOST  DECIMAL(15,2)  NOT NULL,
	PS_COMMENT     VARCHAR(199) NOT NULL );

ALTER TABLE public.partsupp OWNER TO tpch;

--
-- Name: region; Type: TABLE; Schema: public; Owner: tpch
--

CREATE TABLE public.REGION  ( 
	R_REGIONKEY  INTEGER NOT NULL,
	R_NAME       CHAR(25) NOT NULL,
	R_COMMENT    VARCHAR(152));

ALTER TABLE public.region OWNER TO tpch;

--
-- Name: supplier; Type: TABLE; Schema: public; Owner: tpch
--

CREATE TABLE public.SUPPLIER ( 
	S_SUPPKEY     INTEGER NOT NULL,
	S_NAME        CHAR(25) NOT NULL,
	S_ADDRESS     VARCHAR(40) NOT NULL,
	S_NATIONKEY   INTEGER NOT NULL,
	S_PHONE       CHAR(15) NOT NULL,
	S_ACCTBAL     DECIMAL(15,2) NOT NULL,
	S_COMMENT     VARCHAR(101) NOT NULL);

ALTER TABLE public.supplier OWNER TO tpch;

--
-- Name: customer customer_pk; Type: CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.customer
ADD CONSTRAINT customer_pk PRIMARY KEY (c_custkey);


--
-- Name: lineitem lineitem_pk; Type: CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.lineitem
ADD CONSTRAINT lineitem_pk PRIMARY KEY (l_linenumber, l_orderkey);


--
-- Name: nation nation_pk; Type: CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.nation
ADD CONSTRAINT nation_pk PRIMARY KEY (n_nationkey);


--
-- Name: orders orders_pk; Type: CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.orders
ADD CONSTRAINT orders_pk PRIMARY KEY (o_orderkey);


--
-- Name: part part_pk; Type: CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.part
ADD CONSTRAINT part_pk PRIMARY KEY (p_partkey);


--
-- Name: partsupp partsupp_pk; Type: CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.partsupp
ADD CONSTRAINT partsupp_pk PRIMARY KEY (ps_partkey, ps_suppkey);


--
-- Name: region region_pk; Type: CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.region
ADD CONSTRAINT region_pk PRIMARY KEY (r_regionkey);


--
-- Name: supplier supplier_pk; Type: CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.supplier
ADD CONSTRAINT supplier_pk PRIMARY KEY (s_suppkey);


--
-- Name: customer_nation_fkidx; Type: INDEX; Schema: public; Owner: tpch
--

CREATE INDEX customer_nation_fkidx ON public.customer USING btree (c_nationkey);


--
-- Name: idx_lineitem_orderkey_fkidx; Type: INDEX; Schema: public; Owner: tpch
--

CREATE INDEX idx_lineitem_orderkey_fkidx ON public.lineitem USING btree (l_orderkey);


--
-- Name: lineitem_part_supp_fkidx; Type: INDEX; Schema: public; Owner: tpch
--

CREATE INDEX lineitem_part_supp_fkidx ON public.lineitem USING btree (l_partkey, l_suppkey);


--
-- Name: nation_regionkey_fkidx; Type: INDEX; Schema: public; Owner: tpch
--

CREATE INDEX nation_regionkey_fkidx ON public.nation USING btree (n_regionkey);


--
-- Name: order_customer_fkidx; Type: INDEX; Schema: public; Owner: tpch
--

CREATE INDEX order_customer_fkidx ON public.orders USING btree (o_custkey);


--
-- Name: partsupp_part_fkidx; Type: INDEX; Schema: public; Owner: tpch
--

CREATE INDEX partsupp_part_fkidx ON public.partsupp USING btree (ps_partkey);


--
-- Name: partsupp_supplier_fkidx; Type: INDEX; Schema: public; Owner: tpch
--

CREATE INDEX partsupp_supplier_fkidx ON public.partsupp USING btree (ps_suppkey);


--
-- Name: supplier_nation_fkidx; Type: INDEX; Schema: public; Owner: tpch
--

CREATE INDEX supplier_nation_fkidx ON public.supplier USING btree (s_nationkey);


--
-- Name: customer customer_nation_fk; Type: FK CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.customer
ADD CONSTRAINT customer_nation_fk FOREIGN KEY (c_nationkey) REFERENCES public.nation(n_nationkey);


--
-- Name: lineitem lineitem_order_fk; Type: FK CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.lineitem
ADD CONSTRAINT lineitem_order_fk FOREIGN KEY (l_orderkey) REFERENCES public.orders(o_orderkey);


--
-- Name: lineitem lineitem_partsupp_fk; Type: FK CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.lineitem
ADD CONSTRAINT lineitem_partsupp_fk FOREIGN KEY (l_partkey, l_suppkey) REFERENCES public.partsupp(ps_partkey, ps_suppkey);


--
-- Name: nation nation_region_fk; Type: FK CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.nation
ADD CONSTRAINT nation_region_fk FOREIGN KEY (n_regionkey) REFERENCES public.region(r_regionkey);


--
-- Name: orders order_customer_fk; Type: FK CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.orders
ADD CONSTRAINT order_customer_fk FOREIGN KEY (o_custkey) REFERENCES public.customer(c_custkey);


--
-- Name: partsupp partsupp_part_fk; Type: FK CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.partsupp
ADD CONSTRAINT partsupp_part_fk FOREIGN KEY (ps_partkey) REFERENCES public.part(p_partkey);


--
-- Name: partsupp partsupp_supplier_fk; Type: FK CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.partsupp
ADD CONSTRAINT partsupp_supplier_fk FOREIGN KEY (ps_suppkey) REFERENCES public.supplier(s_suppkey);


--
-- Name: supplier supplier_nation_fk; Type: FK CONSTRAINT; Schema: public; Owner: tpch
--

ALTER TABLE ONLY public.supplier
ADD CONSTRAINT supplier_nation_fk FOREIGN KEY (s_nationkey) REFERENCES public.nation(n_nationkey);


--
-- PostgreSQL database dump complete
--

