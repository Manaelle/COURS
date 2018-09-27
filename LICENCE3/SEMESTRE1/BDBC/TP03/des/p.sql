CREATE TABLE t1(a INTEGER, b INTEGER, c INTEGER, d INTEGER,  PRIMARY KEY(a));

CREATE TABLE t2(a INTEGER, b INTEGER, c INTEGER, d INTEGER,  PRIMARY KEY(a));

CREATE TABLE t3(a INTEGER, b INTEGER, c INTEGER, d INTEGER,  PRIMARY KEY(a));

INSERT INTO t1(a,b,c,d) VALUES (1,3,2,2);

INSERT INTO t1(a,b,c,d) VALUES (2,2,2,2);

INSERT INTO t1(a,b,c,d) VALUES (3,1,0,1);

INSERT INTO t2(a,b,c,d) VALUES (1,3,0,2);

INSERT INTO t2(a,b,c,d) VALUES (2,2,1,0);

INSERT INTO t2(a,b,c,d) VALUES (3,1,1,0);

INSERT INTO t3(a,b,c,d) VALUES (1,3,1,1);

INSERT INTO t3(a,b,c,d) VALUES (2,2,2,0);

INSERT INTO t3(a,b,c,d) VALUES (3,1,2,1);

CREATE VIEW v10(a,b,c,d) AS ( SELECT DISTINCT t1.d,t1.a,t1.b,t1.c FROM t1 WHERE t1.d > 2 ) UNION ( SELECT ALL t1.a,t1.d,t1.b,t1.b FROM t1 WHERE t1.a > 1 );

CREATE VIEW v7(a,b,c,d) AS ( SELECT DISTINCT t2.b,t2.c,t2.a,t2.c FROM t2 WHERE t2.b <= 2 ) EXCEPT ( SELECT DISTINCT t2.a,t2.d,t2.b,t2.c FROM t2 WHERE t2.d < 1 );

CREATE VIEW v8(a,b,c,d) AS ( SELECT ALL t1.a,t1.d,t1.b,t1.d FROM t1 WHERE t1.a >= 0 ) EXCEPT ( SELECT ALL t1.b,t1.a,t1.d,t1.d FROM t1 WHERE t1.b >= 0 );

CREATE VIEW v9(a,b,c,d) AS SELECT DISTINCT t3.a,t3.a,t3.d,t3.c FROM t3 WHERE (t3.a = t3.b AND (t3.d <= 1 OR t3.c < 0));

CREATE VIEW v4(a,b,c,d) AS ( SELECT ALL v10.c,v10.c,v10.b,v10.a FROM v10 WHERE v10.d = 1 ) EXCEPT ( SELECT DISTINCT v9.d,v9.d,v9.a,v9.a FROM v9 WHERE v9.a < 0 );

CREATE VIEW v5(a,b,c,d) AS ( SELECT ALL v8.b,v8.d,v8.a,v8.c FROM v8 WHERE v8.d <= 0 ) UNION ALL ( SELECT ALL v7.d,v7.b,v7.b,v7.d FROM v7 WHERE v7.a <= 2 );

CREATE VIEW v6(a,b,c,d) AS ( SELECT DISTINCT v10.b,v10.c,v10.c,v10.c FROM v10 WHERE v10.a = 0 ) UNION ALL ( SELECT ALL v10.d,v10.d,v10.c,v10.d FROM v10 WHERE v10.b > 1 );

CREATE VIEW v2(a,b,c,d) AS ( SELECT DISTINCT v5.c,v5.c,v5.a,v5.c FROM v5 WHERE v5.c = 2 ) UNION ( SELECT ALL v4.a,v4.c,v4.d,v4.d FROM v4 WHERE v4.a > 0 );

CREATE VIEW v3(a,b,c,d) AS ( SELECT ALL v6.a,v6.c,v6.c,v6.a FROM v6 WHERE v6.c <= 2 ) UNION ( SELECT DISTINCT v6.d,v6.c,v6.c,v6.b FROM v6 WHERE v6.b < 0 );

CREATE VIEW v1(a,b,c,d) AS SELECT DISTINCT v2.a,v2.d,v2.b,v3.d FROM v2, v3;

