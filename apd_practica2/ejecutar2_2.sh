#!/bin/bash
st_path="./sudoku_tests"
sb_path=${st_path}"/sudoku_boards"
sc_path=${st_path}"/sudoku_cnfs"
sbt_path=${st_path}"/sudoku_bt_sols"
ss_path=${st_path}"/sudoku_sats"
sst_path=${st_path}"/sudoku_sat_sols"

mkdir $sc_path $sbt_path $ss_path $sst_path

echo -e "\n============================================================"
echo "Authors: Antonio Gallén Recio (735184) & Devid Dokash (780131)"
echo "============================================================"
echo "usage: ./practica2_2 -input <input_file> -output <output_file> -outsol <sol_file> [-sat <results> <n>] [-vet]"
echo "  -input       Sudoku file input.";
echo "  -output      Sudoku cnf file output.";
echo "  -outsol      Sudoku solution file output.";
echo "  -sat         Sat mode. Process the files as SAT ones.";
echo "    <results>  Sat results file.";
echo "    <n>        Sudoku size.";
echo "  -vet             Verbose execution times.";
echo "============================================================"
echo "> Compiling project. . ."
g++ main_2.cpp -std=c++11 -o practica2_2
if [ $? -ne 0 ]; then 
    echo "> Something bad happened. . ."
    exit 1
fi
echo "> Compiled! ! !"
echo "============================================================"
echo "> BE CAREFUL! The script will only work with MiniSAT."
echo "> It's a must to have it installed. MiniSAT Source: https://github.com/master-keying/minisat/"

ms_path="./minisat"
if [ $1 -z "" ]; then 
    if [ ! -f $ms_path ]; then
        read -p "> I see that you didn't provide the MiniSAT path, now is the time: " ms_path
        echo "> This is the path which I will work with: $ms_path" 
    fi
fi

echo "============================================================"
echo "============================================================"
boards=(
    "sudoku_2x2_ez"
    "sudoku_2x2_hard"
    "sudoku_2x2_god"
    "sudoku_3x3_amateur"
    "sudoku_3x3_hard"
    "sudoku_3x3_god"
    "sudoku_4x4_amateur"
    "sudoku_5x5_amateur"
    "sudoku_6x6_amateur"
)
do_back=( 1 1 1 1 1 1 0 0 0 )
echo "> Testing. . ."
for i in ${!boards[@]}; do
    b=${boards[$i]}
    echo "> ${b}"
    if [ ${do_back[$i]} -eq 0 ]; then pass="pass"; else pass=${sbt_path}"/"${b}".sol"; fi

    ./practica2_2 -input ${sb_path}"/"${b}".txt" -output ${sc_path}"/"${b}".cnf" -outsol $pass -vet
    size=$?
    ${ms_path} ${sc_path}"/"${b}".cnf" > ${ss_path}"/"${b}".sat" 2> ${ss_path}"/"${b}".res"
    ./practica2_2 -input ${ss_path}"/"${b}".sat" -output ${sst_path}"/"${b}".sat.sol" -sat ${ss_path}"/"${b}".res" $size -vet
done

rm practica2_2
