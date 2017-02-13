package main

import "fmt"
import "time"

const SIZE int = 512;

func main() {

    var src [SIZE][SIZE]int
    var dest [SIZE][SIZE]int

    fmt.Println("SIZE=", SIZE)
    t1 := time.Now();
    copyAB(src, dest)
    t2 := time.Now();
    fmt.Printf("copyAB - duration: %s\n", t2.Sub(t1))

    t1 = time.Now();
    copyBA(src, dest)
    t2 = time.Now();
    fmt.Printf("copyAB - duration: %s\n", t2.Sub(t1))
}


func copyAB (src[SIZE][SIZE]int, dest[SIZE][SIZE]int) {
    for row := 0; row < SIZE; row++ {
        for col := 0; col < SIZE; col++ {
            dest[row][col] = src[row][col]
        }
    }
}

func copyBA (src[SIZE][SIZE]int, dest[SIZE][SIZE]int) {
    for row := 0; row < SIZE; row++ {
        for col := 0; col < SIZE; col++ {
            dest[col][row] = src[col][row]
        }
    }
}
