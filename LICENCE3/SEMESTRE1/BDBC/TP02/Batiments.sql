spool Bureau/COURS/BDBC/Batiments_Reponses


set pagesize 200
set linesize 260
col nom format a20 
col pid format 999

-- 1
select P.pid, count(R.objet) as nbReservation, avg(nvl(R.nbPers,0)) as moyPers
from batiments.LesReunions R
right join batiments.LesPieces P
on R.pid = P.pid
group by P.pid;

-- 2
select distinct eid, pid
from batiments.LesMesures
natural join batiments.LesPieces
join (select eid as eidTPE, min(minT) as minTPE
	from (select eid, pid, min(temp) as minT
		from batiments.LesMesures
		natural join batiments.LesPieces
		group by eid, pid)
	group by eid)
on (temp = minTPE) and (eid = eidTPE)
order by eid, pid;

--select * from batiments.LesPieces;

--3
select distinct nom, prenom
from batiments.LesReunions;

select row_number() over (order by nom) as ID,
nom, prenom from (select distinct nom, prenom
									from batiments.LesReunions)
order by nom;


select pid, eid, 
from batiments.LesPieces
right join 

spool off;
