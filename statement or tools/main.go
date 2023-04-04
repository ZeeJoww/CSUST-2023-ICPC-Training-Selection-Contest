package main

import (
	"fmt"
)

func main() {
	var t int
	_, err := fmt.Scanf("%v\n", &t)

	if err != nil {
		return
	}

	for true {
		var a, b uint64
		_, err := fmt.Scanf("%v %v\n", &a, &b)
		if err != nil {
			break
		}
		fmt.Println(a * b)
	}
}
