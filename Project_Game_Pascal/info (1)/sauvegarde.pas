unit sauvegarde;

Interface

uses initialisation, TypesJeu, sysutils, Crt;

procedure ecrireSauvegarde(p:Personnage;nomS:String);
procedure lireSauvegarde(var p:Personnage;nomS:String);


Implementation


procedure ecrireSauvegarde(p:Personnage; nomS:String);

var fichier: file of Personnage;
	
begin

assign(fichier, nomS); 

rewrite(fichier);
write(fichier,p);
close(fichier);

end; {ecrireSauvegarde}

procedure lireSauvegarde(var p:Personnage;nomS:String); 

var fichier: file of Personnage;
var repereAvant: Real;

begin

	if FileExists(nomS) then
	begin
	assign(fichier,nomS);

	reset(fichier);
	read(fichier,p);
	close(fichier);
	end

else

	begin
	writeln('Il n existe pas de sauvegarde sur cette histoire pour l instant;');
	writeln('Création d une nouvelle partie en cours...');
	delay(2000);
	initPartie(p, repereAvant);
	
	

	end;


end; {lireSauvegarde}

begin
end.
