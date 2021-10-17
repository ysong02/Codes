--LIU Yiheng, SONG Yuxuan G3

import Prelude hiding (Left, Right)
import System.IO
import Data.Char
import Control.Monad

--Définir le type Instruction
data Instruction = Forward Float | Left Float | Right Float | Repeat Int [Instruction] deriving (Read, Show)

--séparer les instructions et avoir les calculations pour obtenir les déplacements 
readInstructions :: [Instruction] -> [(Float, Float)] -> Float -> [(Float, Float)]
readInstructions [] pos _ = pos
--les deplacements sont dans pos 
readInstructions (x:xs) pos angle = case x of
--seulement Forward va faire un déplacement, d'après la fonction forward on peut l'ajoute dans pos
     Forward i -> readInstructions xs (pos ++ [forward angle i]) angle
-- left et Right juste change la direction, comme angle-i pour Left et angle+i pour Right
     Left i -> readInstructions xs pos (angle-i)
     Right i -> readInstructions xs pos (angle+i)
--Nous répétons d'écrire les codes qui sont dans l'instruction Repeat le nombre de fois requis par l'instruction
     Repeat i instruction -> readInstructions addxs pos angle
        where addxs = (take (i* length(instruction)) (cycle instruction)) ++ xs

--pour calculer le déplacement
forward :: Float -> Float -> (Float, Float)
forward angle i = (x, y)
     where a = angle * (pi/180)
           x = i * cos a
           y = i * sin a

--calculer les positions par la somme de déplacement et sortir strings dans format de svg
outputStr :: [(Float,Float)] -> (Float,Float) -> String
outputStr [] _ = ""
outputStr (x:xs) (x0,y0) = t ++ outputStr xs (nx,ny)
  where nx = x0 + fst x
        ny = y0 + snd x
        t = concat["<line x1=\"", show(x0) , "\" y1=\"", show(y0),"\" x2=\"", show(nx), "\" y2=\"", show(ny), "\" stroke=\"red\" />\n"]

--ajouter le début et la fin de format svg
joint :: String -> String
joint t = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"200\" height=\"200\">\n<title>Exemple</title>\n"++t++"</svg>"

main = do
--lire le document .logo
     contents <- hGetLine stdin
--écrire un document .svg
     hPutStr stdout (joint ( outputStr (readInstructions (read contents :: [Instruction]) [] 0 ) (100, 100) ))
