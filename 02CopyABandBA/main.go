package main

import "fmt"
import "time"

const SIZE int = 512;

// https://coderwall.com/p/cp5fya/measuring-execution-time-in-go
func timeTrack(start time.Time, name string) {
    elapsed := time.Since(start)
    fmt.Printf("%s took %s\n", name, elapsed)
}

func main() {

    var src [SIZE][SIZE]int
    var dest [SIZE][SIZE]int

    fmt.Println("SIZE=", SIZE)

    copyAB(src, dest)
    copyBA(src, dest)
}


func copyAB (src[SIZE][SIZE]int, dest[SIZE][SIZE]int) {
    defer timeTrack(time.Now(), "copyAB")
    for row := 0; row < SIZE; row++ {
        for col := 0; col < SIZE; col++ {
            dest[row][col] = src[row][col]
        }
    }
}

func copyBA (src[SIZE][SIZE]int, dest[SIZE][SIZE]int) {
    defer timeTrack(time.Now(), "copyBA")
    for row := 0; row < SIZE; row++ {
        for col := 0; col < SIZE; col++ {
            dest[col][row] = src[col][row]
        }
    }
}
