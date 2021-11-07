program jeu;

uses TypesJeu, menu, initialisation, interaction, crt;
var choix : String;

begin

user();
repeat
afficherMenu();
touche(choix);
clrscr();
useMenu(choix);
until choix='6';
exit;
end.
