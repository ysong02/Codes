unit initialisation;

Interface

uses TypesJeu, Crt, interaction;

procedure regles();
procedure user();
procedure lireUser( var nomJ: String);
procedure character( var perso:Personnage);
procedure choixHistoire(var nomH, nomM,nomF, nomS:String);  
procedure initPartie(var p:Personnage; var repereAvant: Real);


Implementation

procedure regles();

var fichier:Text;
var chaine : String;
var choix:Integer;

begin
assign(fichier,'txt/regles.txt');
reset(fichier);

    repeat
	readln(fichier, chaine);
	writeln(chaine);		
	
	until eof(fichier);
	
close(fichier);

repeat
writeln('Entrez 0 pour retourner au menu');
readln(choix);
until (choix=0);

end;

procedure user();

var nomJ: String;
	fichier:Text;
begin
assign(fichier,'txt/user.txt');
writeln('Bienvenue dans ALAKAZAM, entrez votre nom');
readln(nomJ);
rewrite(fichier);
write(fichier,nomJ);
close(fichier);
end; {user}

procedure lireUser( var nomJ: String);
var fichier:Text;
begin
assign(fichier,'txt/user.txt');

reset(fichier);
read(fichier,nomJ);
close(fichier);

end; {lireUser}

procedure character( var perso:Personnage);
var n1,n2,n3,i : Integer;
begin
perso.repere:=0;
randomize();
n1:=random(2);
	repeat 
		n2:=random(2);
	until(n1<>n2);
	case n1 of
	0: perso.cara[1]:= 'Rapide';
	1: perso.cara[1]:= 'Intelligent';
	
	end;
	
	case n2 of 
	0: perso.cara[2]:= 'Lent';
	1: perso.cara[2]:= 'Simplet';
	
	end;
	
n3:=random(3);

	case n3 of 
	0: perso.vieMax:=80;   
	1: perso.vieMax := 100;
	2: perso.vieMax := 120;
	
	end;
	
perso.ptVie:=perso.vieMax;

for i:=1 to 3 do 
	perso.invent[i]:= false; 
			
end; {character}



procedure initPartie(var p:Personnage; var repereAvant: Real);

begin

repereAvant :=0;

	writeln('Bienvenue dans cette nouvelle partie.');
	delay(1000);
	writeln('Pour commencer, nous allons initialiser les caractéristiques de votre personnage.');
		character(p);
	delay(1000);
	writeln('Vous avez obtenu les caratéristiques suivantes : ');
		writeln(p.cara[1],', ',p.cara[2]);
	delay(1000);
	writeln('Vous avez ', p.ptVie , ' points de vie'); 
	delay(1000);
	writeln();  

end; {initPartie}




procedure choixHistoire(var nomH, nomM,nomF,nomS:String);  

var fichier: Text;
var chaine, choix,nomJ: String;
var chiffre :Integer;

begin
lireUser(nomJ);
nomH:='txt/histoire/histoire';
nomM:='txt/monstres/monstres';
nomF:='txt/fin/fin';   
nomS:='txt/sauvegarde/sauv';        

writeln('Veuillez choisir une histoire parmi les propositions suivantes');

assign(fichier,'txt/resume.txt');
reset(fichier);

    repeat
	read(fichier, chiffre);
	write(chiffre, ' : ');
	readln(fichier, chaine);
	writeln(chaine);		
	
	until eof(fichier);
	
touche(choix);
nomH:=nomH+choix+'.txt';
nomM:=nomM+choix+'.txt';
nomF:=nomF+choix+nomJ+'.txt';   
nomS:=nomS+choix+nomJ+'.txt';   

	  			
			
close(fichier);


end; {choixHistoire}




begin
end.
