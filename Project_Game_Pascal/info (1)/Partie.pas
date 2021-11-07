unit Partie;



Interface

uses Crt, TypesJeu, SysUtils, labyrinthe, initialisation, Keyboard ,sauvegarde;

procedure deroulementPartie(nomH, nomM, nomF,nomS:String; nouvP:Boolean);
procedure texte(nomH: String; c:Real; var testSigne: String);  
procedure de(var i:Integer); 

const PTPOTION=50;

Implementation

uses endings;



function repere(repereAvant:Real; repereChoix:Integer):Real;
var c:Real;
begin
c:= repereAvant;
c:=c*10;
c:=c+repereChoix;

repere:=c;


end; {repere}






procedure choix(repereAvant:Real; var RepereApres:Real);

var reponse:Integer;

begin

readln(reponse);

	repereApres:=repere(repereAvant, reponse);
	
		
end; {choix}



procedure texte(nomH: String; c:Real; var testSigne: String); 

var fichier: Text;
var chaine: String;
var chiffre: Real;

begin


assign(fichier,nomH);
reset(fichier);

	repeat
	read(fichier, chiffre);
	readln(fichier, chaine);
	readln(fichier, testSigne);		
	
	until (chiffre=c);
	
	writeln(chaine);                 
	
		
			
			
close(fichier);

end; {texte}

procedure mechant(nomM:String ; c:Real;var ennemi:Monstre);
var fichier: Text;
var chaine: String;
var chiffre: Real;
var vie : Integer;

begin 

assign(fichier,nomM);
reset(fichier);

	repeat
	readln(fichier, chiffre);
	readln(fichier, chaine);
	readln(fichier, vie);	
	until (chiffre=c);
	
	ennemi.cara:=chaine;
	ennemi.ptVie:=vie;
	 
	
close(fichier);

end; {mechant}

procedure potion(var p:Personnage);

var utilise, vieTest :Integer;
begin

writeln('Vous avez une potion qui peut vous rendre ' ,PTPOTION, ' points de vie. Entrez 1 pour l utiliser ou 2 pour continuer sans la boire.');
readln(utilise);

if utilise=1 then 
begin

p.invent[3]:=False;
vieTest:=p.ptVie+50;

	if vieTest>p.vieMax then
	p.ptVie:=p.vieMax
	
	else
	p.ptVie:=vieTest;	
	
writeln('Vous avez maintenant ', p.ptVie, ' points de vie.');

end

else
writeln('Vous conservez votre potion');



end; {potion}


procedure de(var i:Integer);

var x,y:Integer;

begin

x:=wherex();
y:=wherey();

i:=0;

repeat

if i<6 then
i:=i+1

else
i:=1;

write(i);
delay(100);
gotoxy(x,y);

until KeyPressed();


end; {de}



procedure combat(degats, condition : Integer; var c : Real; var death, mortM : Boolean; var p:Personnage; var ennemi: Monstre);

var n,cont :Integer;

begin

writeln('Le combat commence !');

repeat  


if ((p.invent[3]) and (p.ptVie < p.vieMax)) then 
potion(p);

writeln('Entrez 0 pour lancer le dé');
read(cont);
if cont = 0 then
de(n);



writeln('Vous obtenez un ', n);

if (n>=condition) then 
	begin
	ennemi.ptVie := ennemi.ptVie-degats;
	delay(2000);
	TextColor(Green);
	writeln('Vous infligez ',degats,' points de dégâts à votre ennemi');
	NormVideo();
	end
else
	begin
	p.ptVie:=p.ptVie-20;
	delay(2000);
	TextColor(Red);
	writeln('Votre ennemi vous inflige 20 pts de dégâts');
	 NormVideo();
	end;
	
delay(2000);
	
if p.ptVie <=0 then
	begin
	death:= true;
	TextColor(Red);
	writeln(' Vous êtes mort au combat ');
	 NormVideo();
	
	end
else
	if ennemi.ptVie <=0 then
		begin
		mortM:= true;
		TextColor(Green);
		writeln('Vous avez vaincu votre ennemi');
		NormVideo();
		c:=repere(c,3);
			if (p.ptVie<=(p.vieMax div 2)) then
			begin
			writeln('Bravo ! Vous récuperez 20 points de vie');
			p.ptVie:=p.ptVie +20;
			end;
		end
	else
		writeln('Vous avez actuellement ', p.ptVie,' points de vie et votre ennemi en a ', ennemi.ptVie);

writeln();
delay(1500);
until (death or mortM);


end; {combat}

procedure calculDegats(var degats: Integer; p: Personnage);
begin
if p.invent[2] = True then
	begin
	degats:=40;
	writeln('Vous avez une arme : Vous infligerez ',degats,' points de degats à votre ennemi');
	end
else
	begin
	degats:=20;
	writeln('Vous n avez pas d arme : Vous infligerez seulement ',degats,' points de degats à votre ennemi');
end;

end;{calculDegats}


procedure choixCombat(nomM : String ; var c: Real;  var death : Boolean; var p: Personnage);   

var condition, degats,choice :Integer;
	ennemi : Monstre;
	mortM, sortie: Boolean;
	
begin
	
mortM:= false;
mechant(nomM, c,ennemi);
writeln();
writeln('Vous avez actuellement ', p.ptVie,' points de vie et votre ennemi en a ', ennemi.ptVie);
delay(2000);
	if (((p.cara[1] = 'Intelligent') and (ennemi.cara = 'Simplet')) or ((p.cara[1]='Rapide') and (ennemi.cara = 'Lent'))) then
		begin 
		condition:= 2;
		writeln('Vous avez un avantage sur votre ennemi car il est ', ennemi.cara,' , vous devez obtenir ',condition,' ou plus au lancer de dé pour attaquer');
		end
	else
	
		if (((p.cara[2] = 'Simplet') and (ennemi.cara = 'Intelligent')) or ((p.cara[2]='Lent') and (ennemi.cara = 'Rapide'))) then
			begin 
			condition:= 4;
			writeln('Vous avez un désavantage sur votre ennemi car il est ', ennemi.cara,' , vous devez obtenir ',condition,' ou plus au lancer de dé pour attaquer');
			end
		else 
			begin
			condition:=3;
			writeln('Vous devez obtenir ',condition,' ou plus au lancer de dé pour attaquer');
			end;
writeln();	
delay(1000);
calculDegats(degats,p);
writeln();
delay(1000);
writeln('Vous avez 2 options: vous battre (1) ou prendre la fuite (2). Que voulez vous faire?');
readln(choice);


randomize();

if choice=1 then

	combat(degats, condition, c, death, mortM, p, ennemi);

 
if choice=2 then
	begin
		clrscr();
		fuite(death, sortie);
			if sortie then
				c:=repere(c,4);
	end;
				
			
		
end; {choixCombat}



procedure cle(var c: Real; var p: Personnage); 

begin

if p.invent[1]= True then
	begin
	c:=repere(c,6);
	p.invent[1]:=False;
	end
else
c:=repere(c,7);

end; {cle}

procedure test(nomM :String ; testSigne:String; var death :Boolean; var ending:Boolean; var repereAvant : Real; var c :Real; var p: Personnage);
var numObjet : Integer;  
begin

if (testSigne='@')        
		then 
		 
		choixCombat(nomM, c,death,p);     
			
	if (testSigne='#')
		then
			
		death:=True;
		
	if (testSigne=';')
		then 
		
        choix(repereAvant,c);
        
    if (testSigne='*')
		then
		
		ending:=True;
		
	if (testSigne[1]='!')
		then
		begin
			delete(testSigne,1,1);
			c:=StrToFloat(testSigne);  
		end; 
		
	if (testSigne[1]='/') 							
		then
		begin
			delete(testSigne,1,1);
			numObjet:=StrToInt(testSigne);  
			
			if p.invent[numObjet]= True then
			writeln('Vous possédez déjà cet objet ! Vous vous débarassez de l ancien.');
			
			p.invent[numObjet]:= True;
			c:=repere(c,5);
			
			
			
		end; 
		
	if (testSigne='$')
		then
		cle(c,p); 
		   
end; {test}



procedure deroulementPartie(nomH, nomM, nomF,nomS:String; nouvP:Boolean);

var repereAvant,c, retour: Real;
var testSigne: String;
var death, ending:Boolean;
var p : Personnage;

begin
death:=False;
ending:=False;

if nouvP then
initPartie(p, repereAvant);

if nouvP=false then
begin
lireSauvegarde(p,nomS);
repereAvant:=p.repere;
c:=repereAvant;
end;

repeat 
texte(nomH, repereAvant,testSigne);
test(nomM, testSigne, death, ending, repereAvant, c,p);
	
repereAvant:=c;
p.repere:=c;
ecrireSauvegarde(p,nomS);

until (death or ending);        

if death then
	begin
		TextColor(Red);
		writeln('Votre partie est un échec !');        
		fichierFin(c,nomF);	
		NormVideo();
	end;

if ending then
	begin
		TextColor(Green);
		writeln('Votre partie est un succès, félicitations !');
		fichierFin(c,nomF);
		NormVideo();
	end;

writeln();
repeat
writeln('Entrez 0 pour retourner au menu');
readln(retour);
until (retour=0);


end; {deroulementPartie}


begin
end.


