#!/bin/bash
echo -e "\n============================================================"
echo "Authors: Antonio Gallén (735184), Devid Dokash (780131)"
echo "============================================================"
echo "Usage: ./practica1 { <size> <top_number> | <file> } "
echo "  <size>          Size of the vector to generate."
echo "  <top_number>    Max number to generate."
echo "  <file>          File with vector."
echo "============================================================"
echo "> Compiling project. . ."
g++ practica1.cpp -std=c++11 -o practica1
echo -e "\n> 1st test. #Size: 1000000, Top number: 1000000"
./practica1 1000000 1000000
echo

read -p "Press any key. . ." 
echo -e "\n> 2nd test. #Size: 1000000, Top number: 1000000000"
./practica1 1000000 1000000000
echo

read -p "Press any key. . ." 
echo -e "\n> 3rd test. File: death.txt"
./practica1 death.txt
echo;

read -p "Press any key. . ." 
echo -e "\n> 4th test. File: negative.txt"
./practica1 negative.txt
echo

read -p "Press any key. . ." 
echo -e "\n> 5th test. File: pending.txt"
./practica1 pending.txt
echo

read -p "Press any key. . ." 
echo -e "\n> 6th test. File: pending.txt"
./practica1 positive.txt
echo

read -p "Press any key. . ." 
echo -e "\n> 7th test. File: pending.txt"
./practica1 test.txt
echo