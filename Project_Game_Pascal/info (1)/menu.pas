unit menu;

Interface

uses TypesJeu, Partie, Crt, initialisation;

procedure afficherMenu();
procedure useMenu(var choix : String);

Implementation

uses endings;

procedure afficherMenu();

var nomJ:String; 


begin
lireUser(nomJ);

clrscr();
writeln('Bienvenue dans ALAKAZAM ',nomJ);
writeln('Que voulez vous faire ?');

writeln('0 : Règles du jeu');
writeln('1 : Nouvelle partie');
writeln('2 : Reprendre partie');
writeln('3 : Fins obtenues');
writeln('4 : Effacer les fins obtenues');
writeln('5 : Changer de joueur');
writeln('6 : Quitter le jeu');     


end; {afficherMenu}

procedure useMenu( var choix : String);

var nomH, nomM, nomF, nomS:String; 

var nouvP:Boolean;

begin

if choix='0' then
regles();

if choix='1' then
begin
nouvP:=True;
choixHistoire(nomH, nomM, nomF, nomS);
clrscr();
deroulementPartie(nomH, nomM, nomF,nomS,nouvP);   
end;

if choix='2' then
begin
nouvP:=False;
choixHistoire(nomH, nomM, nomF, nomS);
clrscr();
deroulementPartie(nomH, nomM, nomF,nomS,nouvP); 
end;

if choix='3' then
begin
choixHistoire(nomH, nomM, nomF, nomS);
afficherFin(nomF,nomH);
end;

if choix='4' then
begin
choixHistoire(nomH,nomM,nomF, nomS);
clrscr();
confirmation(nomF);
end;

if choix='5' then

user()


else

write('Au revoir');

end; {useMenu}


begin
end.
