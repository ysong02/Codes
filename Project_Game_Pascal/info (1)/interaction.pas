unit interaction;

Interface

uses Keyboard;

procedure touche(var resultat:String);

Implementation

procedure touche(var resultat:String);

var key:TKeyEvent;

begin

InitKeyboard();
key:=GetKeyEvent();
key:=TranslateKeyEvent(key);
DoneKeyboard();

resultat:=(KeyEventToString(key));

end; {touche}

begin
end.
