package main

import (
	"bufio"
	"fmt"
	"io"
	"strings"
	"net"
	"os"
	"strconv"
)
const BUFFERSIZE = 1024


func getPort() (int) {

	if len(os.Args) != 3 {
		fmt.Printf("Usage: go run client.go <portnumber> <FileName> \n")
		os.Exit(1)
	} else {
		fmt.Printf("#DEBUG ARGS Port Number : %s\n", os.Args[1])
		portNumber, err := strconv.Atoi(os.Args[1])
		if err != nil {
			fmt.Printf("Usage: go run client.go <portnumber> <FileName> \n")
			os.Exit(1)
		} else {
			return portNumber

		}

	}
	//Should never be reached
	return -1
}

func getName() string{ // pour récupérer le nom du fichier en argument
	if len(os.Args) != 3 { // on vérifie qu'un argument a bien été donné
		fmt.Println("Erreur: il manque des arguments!")
		os.Exit(1)
	}else{
		name := os.Args[2]
		return name
	}
	return ""
}

func main() {
	port := getPort()
	fileName := getName()
	fmt.Printf("#DEBUG DIALING TCP Server on port %d\n", port)
	portString := fmt.Sprintf("127.0.0.1:%s", strconv.Itoa(port))
	fmt.Printf("#DEBUG MAIN PORT STRING |%s|\n", portString)

	conn, err := net.Dial("tcp", portString)
	if err != nil {
		fmt.Printf("#DEBUG MAIN could not connect\n")
		os.Exit(1)
	} else {

		defer conn.Close()
		fmt.Printf("#DEBUG MAIN connected\n")
	}

	file, err := os.Open(fileName)
	//handle errors while opening
	if err != nil {
		fmt.Println(err)
		os.Exit(2)
	}

	//reader := bufio.NewReader(conn)
	fmt.Printf("#DEBUG MAIN connected\n")

	//handle errors while opening
	if err != nil {
		fmt.Println(err)
		os.Exit(2)
	} else {

		defer file.Close()
		reader := bufio.NewReader(conn)
		sendBuffer := make([]byte, BUFFERSIZE)
		fmt.Println("Start sending file!")
		for {
			_, err = file.Read(sendBuffer)
			if err == io.EOF {
				break
			}
			conn.Write(sendBuffer)
		}

		fmt.Println("File has been sent, waiting for the result!")



		resultFile := "ResultClient.txt"
		f, err := os.Create(resultFile)
		if err != nil {
			fmt.Println("Fail to open file:", err)
			os.Exit(2)
		}
		defer f.Close()
		for {
			resultString, err := reader.ReadString('\n')
			if err != nil || resultString == ". . .\n"{
				fmt.Printf("Reception finie, fermeture de la connexion!!\n")
				break
			}
			resultString = strings.TrimSuffix(resultString, "\n")
			//fmt.Printf("#DEBUG server replied 1 : |%s|\n", resultString)
			//fmt.Println(resultString)
			n, err := f.WriteString(resultString+"\n")
			if(err != nil){
				fmt.Printf("Error:",n,err)
			}
		}
	}



}
