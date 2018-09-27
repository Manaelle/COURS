spool Bureau/COURS/BDBC/Transports_Reponses

select distinct salaire
from camions.LesPersonnes
natural join camions.LesSecretaires;

select distinct noC
from camions.LesContrats;

select distinct noC
from camions.LesContrats
group by noC
having count(noTr) >= 2;

select noC
from camions.LesContrats
minus
select noC
from (select noC
	from camions.LesContrats
	group by noC
	having count(noTr) >= 2);
	
select noC
	from (select noC, count(noTr) as nbT
	from camions.LesContrats
	group by noC)
where nbT = 1;

select distinct C1.noC
from camions.LesContrats C1
join camions.LesContrats C2
on (C1.noC = C2.noC) and (C1.noTr <> C2.noTr);

select noP, noTr
from camions.LesContrats;

select noTr, noP, immat, marque
from camions.LesCamions
natural join camions.LesContrats;

select vDep as ville
from camions.LesContrats
union
select vArr as ville
from camions.LesContrats;

select noP, nom, prenom
from (select noP, max (nbK) as maxK
	from camions.LesChauffeurs
	group by noP)
natural join camions.LesPersonnes;

select noP
from (select noP, vDep as ville
		from camions.LesContrats
		union
		select noP, vArr as ville
		from camions.LesContrats)
group by noP
having count(ville) in (select count(ville) as nbV 
						from (select vDep as ville
							from camions.LesContrats
							union
							select vArr as ville
							from camions.LesContrats));
							
select C1.immat
from camions.LesContrats C1
join camions.LesContrats C2
on (C1.immat = C2.immat) and
	(C1.noTr <> C2.noTr) and
	(C1.dateDep < C2.dateDep) and
	(C1.dateDep < C2.dateArr);
							
select immat, dateAchat 
from camions.LesCamions
where dateAchat in (select min(dateAchat) as minD
from camions.LesCamions);


spool off;
