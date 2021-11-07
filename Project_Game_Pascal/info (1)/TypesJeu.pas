unit TypesJeu;

Interface



Type Personnage = record             
cara : array[1..2] of String;
invent: array[1..3] of Boolean; 
ptVie:Integer;
vieMax:Integer;
repere:Real;
end; {Personnage}

Type Monstre = record
cara: String;
ptVie:Integer;
end; {Monstre}

const MAX = 100;
Type Lab = array[1..MAX,1..MAX] of Boolean;


Implementation

begin
end.



