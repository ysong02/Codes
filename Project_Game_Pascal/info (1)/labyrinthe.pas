unit labyrinthe;

Interface

uses Keyboard, Crt, sysutils, TypesJeu;


procedure fuite(var stop:Boolean ; var sortie :Boolean);


Implementation

procedure chargement(nom : String; var l : Lab; var mX, mY, sortieX, sortieY : Integer);
var fic	: Text;
   i, j	: Integer;
   str	: String;
begin
   
   
assign(fic,nom);
reset(fic);
   
	read(fic,mX); 
    readln(fic,mY);    
      
j := 1;
	while (not eof(fic)) do
      begin
		readln(fic,str);
	  for i := 1 to mX do
	    if (str[i] = '1') then
	       l[i][j] := True
	    else 
			begin
			l[i][j] := False;
				if (str[i]='2') then
					begin
					sortieX:=i;
					sortieY:=j;
					end;
			end;	
	 j := j+1;
      end;
  
 
end; {chargement} 


procedure affichage(l : Lab; mX, mY, sortieX, sortieY : Integer);
var i,j:Integer;

begin
GoToXY(1,1);
For j:=1 to mY do
begin
	For i:= 1 to mX do 
	begin
	if l[i][j]=true then
	write('#')
	else write(' ');
	end;
writeln();
end;

GoToXY(sortieX,sortieY);
write('~');

end; {affichage}


procedure jeuLab(l : Lab; mX, mY, sortieX, sortieY : Integer; var sortie : Boolean ; var stop : Boolean);
var key:TKeyEvent;
var posX,posY : Integer;
var posXct, posYct: Integer;
var hr1, hr2, min1, min2, s1, s2, ms : Word;


begin

posX:=2;
posY:=2;
GoToXY(posX,posY);
write('o');

stop:=False;
sortie:=False;
DecodeTime(Time, hr1, min1, s1, ms);

repeat
key:=GetKeyEvent();
posXct:=posX;
posYct:=posY;

	case (KeyEventToString(key)) of
	'Key with scancode 18432': posY:=posY -1;
	'Key with scancode 20480': posY:=posY +1;
	'Key with scancode 19200': posX:=posX -1;
	'Key with scancode 19712': posX:=posX +1;
	end;

	
if (l[posX][posY])=False then
begin
	GoToXY(posXct,posYct);
	write(' ');
	GoToXY(posX,posY);
	write('o');
	GoToXY(posX,posY);
	
end;
if (l[posX][posY])=True then
begin
posX:=posXct;
posY:=posYct;	
	
	end;

DecodeTime(Time, hr2, min2, s2, ms);

if ((s2>s1+15)and(s1<45)) then                   
	stop:=True
	
	else 
	
		if (min2>min1)or(hr2>hr1) then
			stop:=True;
			
if  ((posY=sortieY)and(posX=sortieX)) then
	sortie:=True;
	
	
until sortie or stop;



end; {jeuLab}


procedure choixLabyrinthe(var nom:String);

var i:Integer;
var j:String;

begin

nom:='laby/laby'; 

randomize();
i:=random(8)+1;                

	j:=IntToStr(i);
	nom:=nom+j+'.txt';


end; {choixLabyrinthe}


procedure fuite(var stop:Boolean ; var sortie :Boolean);

var maxX,maxY,sortieX,sortieY: Integer;
var laby	   : Lab;
var nom	       : String;

begin
   InitKeyBoard();
   choixLabyrinthe(nom);
   writeln();
   chargement(nom,laby, maxX, maxY, sortieX, sortieY);
   affichage(laby, maxX, maxY, sortieX, sortieY);
   jeuLab(laby, maxX, maxY, sortieX, sortieY, sortie, stop);
   DoneKeyBoard();
   
 clrscr();
   
	if stop then 
		writeln('Trop tard, votre ennemi vous a rattrapé');

	if sortie then
		writeln('Félicitations');
		

     
end; {fuite}

begin   
end.
