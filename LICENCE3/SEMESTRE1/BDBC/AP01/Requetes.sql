spool Bureau/COURS/BDBC/AP01/Requetes_Reponses

alter session set current_schema=batiments;
 
set pagesize 200
set linesize 260
col nom format a20 
col pid format 999

-- 1
select distinct pid, usage
from LesPieces
natural join LesMesures
where (temp <= 15);


-- 2
WITH
C as (select distinct pid
	from LesMesures
	where temp <= 15)
SELECT pid
FROM LesPieces
MINUS
SELECT pid
FROM C;

-- 3
WITH
D as (select distinct pid, usage, temp
	from LesPieces
	natural join LesMesures
	where temp <= 15),
E as (select pid, usage, count(temp) as nbT
	from D
	group by pid, usage)	
SELECT pid, usage
FROM E
WHERE nbT = 1
ORDER BY pid, usage;

-- 4
WITH
E as (select nom, prenom, row_number() over(order by nom, prenom) as ID
	from (select distinct nom, prenom from LesReunions)),
F as (select R.pid, E.ID
	from LesReunions R
	join E
	on (R.nom = E.nom) AND (R.prenom = E.prenom)),
G as (select pid, eid, ID, count(ID) as nbID
	from LesPieces
	natural left outer join F
	group by pid, eid, ID),
H as (select pid, eid, max(nbID) as maxID
	from G
	group by pid, eid)
SELECT H.pid, H.eid, nvl(nom, 'NONE'), nvl(prenom, 'NONE') , maxID
FROM H
LEFT OUTER JOIN G
ON (H.pid = G.pid) AND (H.eid = G.eid) AND (H.maxID = G.nbID)
NATURAL LEFT OUTER JOIN E
ORDER BY pid, eid, nom, prenom;

-- 5
WITH
O as (select eid, pid, count(dateMes) as nbMes
	from LesPieces
	natural left outer join LesMesures
	group by eid, pid),
P as (select eid, sum(nbMes) as TotalMes
	from O
	group by eid),
M as (select M.eid, pid, maxC
	from (select eid, max(capacite) as maxC
		from LesPieces
		group by eid) M
	join LesPieces
	on (M.eid = LesPieces.eid) AND (M.maxC = capacite))
SELECT eid, TotalMes, pid, maxC
FROM M
NATURAL JOIN P;

--6
select distinct eid, pid, to_char(dateMes, 'DD-MON-YY') as datM, avg(temp) as moyT, max(temp) as maxT, min(temp) as minT
from LesPieces
natural left outer join LesMesures
where (extract(year from dateMes) = 2008)
group by eid, pid, to_char(dateMes, 'DD-MON-YY')
order by eid, pid;


spool off;