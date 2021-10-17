package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
	"time"
	"sort"
	//"sync"
	"io"
)
const BUFFERSIZE = 1024

const infinity int = 99999999
//var wg sync.WaitGroup

type Graph struct{
	from int
	to int
	weight int
}

func getArgs() (int) {

	if len(os.Args) != 2 {
		fmt.Printf("Usage: go run server.go <portnumber>\n")
		os.Exit(0)
	} else {
		fmt.Printf("#DEBUG ARGS Port Number : %s\n", os.Args[1])
		portNumber, err := strconv.Atoi(os.Args[1])

		if err != nil {
			fmt.Printf("Usage: go run server.go <portnumber>\n")
			os.Exit(1)
		} else {
			return portNumber
		}

	}
	//PFR should never be reached
	return -1
}

func main() {
	port := getArgs()
	fmt.Printf("#DEBUG MAIN Creating TCP Server on port %d\n", port)
	portString := fmt.Sprintf(":%s", strconv.Itoa(port))
	fmt.Printf("#DEBUG MAIN PORT STRING |%s|\n", portString)

	ln, err := net.Listen("tcp", portString)
	if err != nil {
		fmt.Printf("#DEBUG MAIN Could not create listener\n")
		panic(err)
	}

	//If we're here, we did not panic and ln is a valid listener

	connum := 1

	for {
		fmt.Printf("#DEBUG MAIN Accepting next connection\n")
		conn, errconn := ln.Accept()

		if errconn != nil {
			fmt.Printf("DEBUG MAIN Error when accepting next connection\n")
			panic(errconn)

		}

		//If we're here, we did not panic and conn is a valid handler to the new connwection

		go handleConnection(conn, connum)
		connum +=1
	}
}

func handleConnection(connection net.Conn, connum int) {

	defer connection.Close()

	// Créer et remplir un fichier GraphRecu qui contient les donnés du graph à utiliser pour construire la matrice d'adjacence
	graphFile := "GraphRecu.txt"
	f, err := os.Create(graphFile)
	connReader := bufio.NewReader(connection)
	defer f.Close()
	if err != nil {
		fmt.Println("Fail to open file:", err)
		os.Exit(2)
	}

	for {
		inputLine, err := connReader.ReadString('\n')

		if err != nil || inputLine==". . .\n" {
			fmt.Printf("#DEBUG %d RCV ERROR no panic, just a client\n", connum)
			break
		}

		//fmt.Println(inputLine)
		n, err := f.WriteString(inputLine)
		if(err != nil){
			fmt.Printf("Error:",n,err)
		}
	}

		//c'est bon
		var graph, noeuds = getData("GraphRecu.txt")
		// c'est bo

		//var graph, noeuds = getData("GraphRecu.txt")
		var matrice = getMatrice(graph, noeuds)
		var number_nodes = len(matrice)

		nbWorker := 10
		numJob := number_nodes
		jobs := make(chan int, numJob)
		result1 := make(chan []int, numJob)
		result2 := make(chan []int, numJob)

		var startTime = time.Now()
		for w := 1; w <= nbWorker; w++ {
			// increment the WaitGroup before starting the worker
			//wg.Add(1)
			go worker(w, jobs, result1, result2, matrice)
			w += 1
		}


		for j := 0; j < numJob; j++ {
			jobs <- j
		}

		// to stop the worker, first close the job channel
		close(jobs)

		// then wait using the WaitGroup
		//wg.Wait()
		var duration = time.Since(startTime)
		fmt.Printf("%v%v\n", "Temps de calcul: ", duration)

		resultFile := "resultServer.txt"
		result, err := os.Create(resultFile)
		defer result.Close()
		if err != nil {
			fmt.Println("Fail to open file:", err)
			os.Exit(5)
		}

		var point int
		for j := 0; j < number_nodes; j++ {
			var distance []int = <-result1
			var way []int = <-result2
			for i := 0; i < number_nodes; i++ {
				if distance[i] == 0 {
					point = i
				}
			}
			result.WriteString("The shortest distance of Node " + strconv.Itoa(point) + " to Node (0 to " + strconv.Itoa(number_nodes-1) + "):\n")
			for i := 0; i < number_nodes; i++ {
				result.WriteString(strconv.Itoa(distance[i]) + "\n")

			}
			result.WriteString("The way of Node " + strconv.Itoa(point) + " to Node (0 to " + strconv.Itoa(number_nodes-1) + ") (except itself):\n")
			for i := 0; i < number_nodes; i++ {
				result.WriteString(affichage(point, way)[i] + "\n")

			}

		}
	result.WriteString(". . .\n")
	file, err := os.Open(resultFile)
	//handle errors while opening
	if err != nil {
		fmt.Println(err)
		os.Exit(2)
	}
	defer file.Close()
	sendBuffer := make([]byte, BUFFERSIZE)
	fmt.Println("Start sending file!")
	for {
		_, err = file.Read(sendBuffer)
		if err == io.EOF {
			break
		}
		connection.Write(sendBuffer)
	}
	fmt.Printf("File sent with success ! ")
}





func Find (slice []int, val int) (bool){ // Tester présence élément dans slice pour éviter les doublons
	for i := range slice{
		if slice[i] == val {
			return true
		}
	}
	return false
}

//Récupère les données d'un fichier et ressort un tableau de Graph
func getData (fileName string)  ([]Graph, []int){
	file, err := os.Open(fileName)

	//handle errors while opening
	if err != nil {
		fmt.Println(err)
		os.Exit(3)
	}
	defer file.Close()

	var brancheGraph []Graph
	var node []int


	//lecture du fichier ligne par ligne
	fileScanner := bufio.NewScanner(file)
	for fileScanner.Scan() { // .Scan par default lit ligne par ligne
		// Pour chaque ligne lue
		for fileScanner.Scan() {
			infos := strings.Split(fileScanner.Text(), " ") // séparation des éléments d'une ligne et rangement dans une slice
			s1, _ := strconv.Atoi(infos[0])
			s2, _ := strconv.Atoi(infos[1])

			found1 := Find(node, s1) // on vérifie que l'on ajoute pas de doublons
			found2 := Find(node, s2)

			if !found1 {
				node = append(node, s1)
			}
			if !found2 {
				node = append(node, s2)
			}

			weight, _ := strconv.Atoi(infos[2]) // conversion du poids qui est de type string en un int
			brancheGraph = append(brancheGraph, Graph{s1, s2, weight})
		}
	}

	sort.Ints(node) // trie le slice des noeuds par ordre alphabétique

	return brancheGraph, node
}
// A partir d'un tableau de graph, ressort une Matrice
func getMatrice(graph []Graph , noeud []int ) [][]int {
	var taille int
	taille = len(noeud)
	var matrice = make([][]int, taille)
	for i:=0;i<taille;i++{
		matrice[i] = make([]int,taille)
	}

	for _, branche := range graph{
		matrice[branche.from][branche.to]=branche.weight
		matrice[branche.to][branche.from]=branche.weight
	}
	for i:=0;i<taille;i++{
		for j:=0;j<taille;j++{
			if i != j {
				if matrice[i][j] == 0 {
					matrice[i][j] = infinity
				}

			}
		}
	}
	return matrice
}

func IsContain(items []int, item int) bool {
	for _, eachItem := range items {
		if eachItem == item {
			return true
		}
	}
	return false
}
// initialiser la distance entre le node choisi et les autres nodes est infini, la distance entre soi-même est 0
func initia(nodes int, number_nodes int) ([]int,[]int) {
	var dist = make([]int,number_nodes)
	var nom_nodes = make([]int,number_nodes)
	for i:=0;i<len(dist);i++{
		nom_nodes[i]=i
		dist[i]=infinity
	}
	dist[nodes]=0
	return dist, nom_nodes
}

// initialiser "unvisited_nodes" comme la liste de nodes au début par default
func init_unvisited(number_nodes int)([]int){
	var unvisited_nodes=make([]int,number_nodes)
	for i:=0;i<number_nodes;i++{
		unvisited_nodes[i]=i
	}
	return unvisited_nodes
}

//Calcule Djikstra， calculer la distance le plus court entre le node choisi et les autres nodes
func dijkstra(matrice [][] int, nodes int) ([]int,[]int) {
	//var visited_nodes= [number_nodes]string{}
	var number_nodes = len(matrice)
	var dist,nom_nodes =initia(nodes, number_nodes)
	var unvisited_nodes = init_unvisited(number_nodes)
	var previous_nodes= make([]int,number_nodes)
	for len(unvisited_nodes)>0 {
		var min int = infinity
		var minindex int = 0
		// chercher le voisin le plus proche
		for i :=0; i<len(dist); i++{
			if dist[i]< min  && IsContain(unvisited_nodes,nom_nodes[i]){
				min = dist[i]
				minindex = i
			}
		}
		//mise à jour la nouvelle distance le plus court si la somme calculée est plus petite et marquer le chemin dans "previous_nodes"
		for k:=0; k<len(matrice); k++{
			if  dist[minindex]+ matrice[minindex][k]<dist[k]{
				dist[k] = dist[minindex]+matrice[minindex][k]
				previous_nodes[k] = nom_nodes[minindex]
			}
		}
		//supprimer le node visité dans "unvisited_nodes"
		var position=0
		for i:=0; i<len(unvisited_nodes);i++{
			if nom_nodes[minindex]==unvisited_nodes[i]{
				position = i
			}
		}
		if len(unvisited_nodes)>1 {
			unvisited_nodes = append(unvisited_nodes[:position], unvisited_nodes[position+1:]...)

		}else{
			unvisited_nodes = []int{}
		}
	}

	return dist, previous_nodes

}

//combiner les chemin de nœud choisi au tout les nœuds
func affichage (node int, previous_nodes []int) []string {
	//fmt.Printf("\x1b[%dm Noeud \x1b[0m\n", 31)
	var chemin = make([]string,len(previous_nodes))
	for i:=0;i<len(previous_nodes);i++{
		var finish_point string=strconv.Itoa(i)
		if i==node{
			continue
		}
		var path string=""
		chemin[i]=paths(path,node,i,previous_nodes)+finish_point
	}
	return chemin
}

//pour créer un chemin de nœud choisi au tout les nœuds separément
func paths (path string,node int,point int,previous_nodes []int)string{
	point = previous_nodes[point]
	if point == node {
		return strconv.Itoa(point) + "->"
	}
	path = strconv.Itoa(point) + "->"
	return paths(path, node, point, previous_nodes)+path
}

// Un certain nombre de worker qui on un tableau de jobs à réaliser
func worker(id int,jobs <-chan int, result1 chan<- []int,result2 chan <-[]int, matrice [][]int){
	//defer wg.Done()
	var result_dist []int
	var result_way  []int
	for j := range jobs{
		//fmt.Println("worker", id, "started  job", j)
		result_dist,result_way= dijkstra(matrice,j)
		result1 <-result_dist
		result2 <-result_way
		//fmt.Println("worker", id, "finished job", j)
	}
}

