unit endings;

Interface

uses sysutils, Partie, Crt;

procedure fichierFin(c:Real; nomF:String);
procedure afficherFin(nomF, nomH: String);
procedure confirmation(nomF:String);

Implementation

procedure ecrireFin(c:Real;nomF:String);

var fichier:Text;

begin

assign(fichier,nomF);
append(fichier);
writeln(fichier, c);
close(fichier);

end; {ecrireFin}





procedure fichierFin(c:Real; nomF:String);

var repere:Real;
var same:Boolean;
var fichier:Text;

begin

assign(fichier,nomF);

if FileExists(nomF) then

	begin

		same:=False;
		reset(fichier);

		repeat

			readln(fichier,repere);
	
				if repere=c then
					same:=True;
	
		until eof(fichier) or same;
	

	close(fichier);


	if same=False then
	ecrireFin(c,nomF)
	
	end

else 
	begin

		rewrite(fichier);
		ecrireFin(c,nomF);
	end;

end; {fichierFin}

procedure afficherFin(nomF, nomH:String);

var fichierF :Text;
var repere, choix:Real;
var test :String;

begin

assign(fichierF,nomF);

if FileExists(nomF) then

begin
clrscr();
reset(fichierF);
writeln('Voici les dernières phrases que vous avez lu lors de vos parties');
writeln();

repeat
readln(fichierF,repere);
texte(nomH, repere, test);
writeln();
writeln();

until eof(fichierF);

close(fichierF);

repeat
writeln('Entrez 0 pour retourner au menu');
readln(choix);
until (choix=0);


end
else 
	
	begin
	writeln('Vous n avez pas obtenu de fins sur cette histoire pour l instant');
	delay(2000);
	
	end;



end; {afficherFin}

procedure effacerFin(nomF:String);


begin


if FileExists(nomF) then

begin
	deleteFile(nomF);
	writeln('Votre historique a bien été effacé');
	delay(2000);
	
end

else

begin
	writeln('Il n existe pas de sauvegarde sur cette histoire pour l instant');
	delay(2000);
	
	end;



end; {effacerFin}

procedure confirmation(nomF:String);

var conf:Integer;

begin

writeln('Entrez 1 pour confirmer la suppression ou 2 pour annuler');
readln(conf);

if conf=1 then
effacerFin(nomF)


end; {confirmation}


begin
end.
