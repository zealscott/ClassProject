create database "scDB";

\c "scDB";

create table "Student" 
(
	"Sno" char(9) unique,
	"Sname" char(5) not null,
	"Ssex" char(2),
	"Sage" smallint,
	"Sdept" char(20),
	primary key("Sno")  
);

create table "Course" 
(
	"Cno" char(4),
	"Cname" char(20) not null,
	"Cpno" char(4),   
	"Credits" smallint,
	primary key("Cno"), 
	foreign key("Cpno") references "Course"("Cno") 
);


create table "SC"  
(
	"Sno" char(9),
	"Cno" char(4),
	"Grade" smallint,
	foreign key("Sno") references "Student"("Sno"), 
    foreign key("Cno") references "Course"("Cno")
);