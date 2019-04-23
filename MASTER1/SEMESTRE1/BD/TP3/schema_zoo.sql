drop table lesemployes ;
drop table lescages ;
drop type ens_cages FORCE;
drop type ens_gardiens FORCE;
drop type tgardien FORCE;
drop type temploye FORCE;
drop type tcage FORCE;

-- Declarer un type tcage
create type tcage;
/

-- Definir une collection libre (nested table) ens_cages de references sur tcage
create type ens_cages as Table of REF tcage;
/

-- Definir un type temploye
create type temploye as object(nom varchar2(20), adresse varchar2(50)) NOT FINAL;
/

-- Definir le sous-type de temploye: tgardien
create type tgardien under temploye(liste_cages ens_cages);
/

-- Creation de la table lesemployes
create table lesemployes of temploye
/

-- Definir une collection libre ens_gardiens de references sur tgardien
create type ens_gardiens as Table of REF tgardien;
/

-- Definir le type tcage (deja declare)
create type tcage as object (identifiant number(3), fonction varchar2(20), num_allee number(3), liste_gardiens ens_gardiens);
/

-- Creation de la table lescages
create table lescages of tcage
	nested table liste_gardiens store as gardiens
/





insert into LesCages values (tcage(11 ,  'fauve'           , 10, ens_gardiens() ));
insert into LesCages values (tcage(1     , 'fosse'         , 1, ens_gardiens() ));
insert into LesCages values (tcage(2     , 'aquarium'      , 1, ens_gardiens() ));
insert into LesCages values (tcage(3     , 'petits oiseaux'        , 2, ens_gardiens() ));
insert into LesCages values (tcage(4     , 'grand aquarium'        , 1, ens_gardiens() ));
insert into LesCages values (tcage(12     , 'fauve'           , 10, ens_gardiens() ));

insert into LesEmployes values (temploye('Verdier'  ,       'Noumea') );
insert into LesEmployes values (tgardien('Spinnard'  ,       'Sartene', ens_cages((select ref(c) from lesCages c where c.identifiant = 11),
																				  (select ref(c) from lesCages c where c.identifiant = 12))));
insert into LesEmployes values (tgardien('Labbe' ,    'Calvi', ens_cages((select ref(c) from lesCages c where c.identifiant = 11))));
insert into LesEmployes values (tgardien('Lachaize' ,       'Pointe a Pitre',ens_cages((select ref(c) from lesCages c where c.identifiant = 1),
																					   (select ref(c) from lesCages c where c.identifiant = 3),
																					   (select ref(c) from lesCages c where c.identifiant = 11),
																					   (select ref(c) from lesCages c where c.identifiant = 12))));
insert into LesEmployes values (temploye('Desmoulins'  , 'Ushuaia') );
insert into LesEmployes values (temploye('Jouanot'   , 'Papeete') );



/