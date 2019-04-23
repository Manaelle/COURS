create or replace trigger Q2_1
before delete or update of noCage on LesGardiens
for each row
begin 
	insert into LesHistoiresAff values(sysdate,:old.noCage,:old.nomE);
end;
/

create or replace trigger Q2_2
after insert or update of noCage on lesAnimaux
declare
	nb integer;
begin
	select count(*) into nb from (
			select nocage, count(distinct type_an) from lesAnimaux 
			group by nocage
			having count(distinct type_an) > 1);
	if (nb>0) then
		raise_application_error(-20001,'cohabitation impossible entre animaux de type different');
	end if;
end;
/

create or replace trigger Q2_3
after insert or update of noCage on lesAnimaux
declare
	nb integer;
begin
	select count(*) into nb from (
			select noCage from lesAnimaux
			MINUS
			select nocage from lesgardiens);
	if (nb>0) then
		raise_application_error(-20001,'un animal ne peut pas être placé dans une cage non gardée');
	end if;
end;
/

create or replace trigger Q2_4
after update or delete of noCage on lesGardiens
declare
	nb integer;
begin
	select count(*) into nb from (
			select noCage from lesAnimaux
			MINUS
			select nocage from lesgardiens);
	if (nb>0) then
		raise_application_error(-20001,'un gardien ne peut pas être retiré de la surveillance d une cage si les animaux qu elle contient se retrouvent non gardes');
	end if;
end;
/

	

/* Test Q1 */
/*update lesgardiens set nocage = 2 where (nocage=12) and (nomE = 'Lachaize');
update lesgardiens set nocage = 12 where nocage = 2 and nomE = 'Lachaize';*/

/* Test Q2 */
/* KO */
/*insert into LesAnimaux values ('Guillaume', 'hermaphrodite', 'poulpe', 'aquarium', 'allemagne', 1986, 11, 42);*/
/* OK */
/*insert into LesAnimaux values ('Guillaume', 'hermaphrodite', 'leopard', 'aquarium', 'allemagne', 1986, 11, 42);*/
/*delete from lesanimaux where noma = 'Guillaume';*/

/* Test Q3 */
/* KO */
/*insert into lesanimaux values('Guillaume','hermaphrodite','leopard','aquarium','allemagne',1986,51,42);*/
/* OK */
/*update lesgardiens set nocage = 1 where (nocage=4) and (nomE = 'Lachaize');*/
/*insert into lesanimaux values('Guillaume','hermaphrodite','leopard','aquarium','allemagne',1986,11,42);*/


/* Test Q4 */
/* KO */
/*update lesgardiens set noCage = 2 where (nocage=1) and nomE = 'Lachaize';*/
/* OK */
/*update lesGardiens set noCage = 2 where nocage = 12 and nomE = 'Lachaize';*/


