#!/bin/bash
echo -e "\n================================================================"
echo "Authors: Devid Dokash (780131)"
echo "================================================================"
echo "Usage: ./practica3 <repetitions> t<n> [t5 options]"
echo "  <repetitions> ...... Number of repetitions"
echo "  t<n> ............... Number of test: 1,2,3,4 or 5."
echo "  [t5 options] ....... Options for test 5."
echo "    <max vertices> ... Graph max vertices." 
echo "    <max_edges> ...... Graph max edges."
echo "    <max_weight> ..... Max possible weight to generate."
echo "    <max_conns> ...... Max possible connections per vertice." 
echo "    <initial_prob> ... Initial probability to generate an edge."
echo "    <decrease_coef> .. Probability decrease coefficient.";
echo "  verbose ............ Shows extra info inside the execution."
echo "================================================================"
echo "> Compiling project. . ."
g++ practica3.cpp -std=c++11 -o practica3
echo "================================================================"
echo "> 1st test. Size: 4 vertices & 4 edges. 1 repetitions:"
./practica3 1 t1
echo "================================================================"
echo "> 1st test. Size: 4 vertices & 4 edges. 10 repetitions:"
./practica3 10 t1
echo "================================================================"
echo "> 1st test. Size: 4 vertices & 4 edges. 100 repetitions:"
./practica3 100 t1
echo "================================================================"
echo "> 1st test. Size: 4 vertices & 4 edges. 1000 repetitions:"
./practica3 1000 t1
echo "================================================================"
echo "> 1st test. Size: 4 vertices & 4 edges. 10000 repetitions:"
./practica3 10000 t1
echo "================================================================"
echo "> 2nd test. Size: 4 vertices & 5 edges (one loop). 1 repetitions:"
./practica3 1 t2
echo "================================================================"
echo "> 2nd test. Size: 4 vertices & 5 edges (one loop). 10 repetitions:"
./practica3 10 t2
echo "================================================================"
echo "> 2nd test. Size: 4 vertices & 5 edges (one loop). 100 repetitions:"
./practica3 100 t2
echo "================================================================"
echo "> 2nd test. Size: 4 vertices & 5 edges (one loop). 1000 repetitions:"
./practica3 1000 t2
echo "================================================================"
echo "> 2nd test. Size: 4 vertices & 5 edges (one loop). 10000 repetitions:"
./practica3 10000 t2
echo "================================================================"
echo "> 3rd test. Size: 10 vertices & 8 edges. 1 repetitions:"
#./practica3 1 t3 verbose
echo "Este test explota ya que el comportamiento es indefinido con grafos de multiples grafos inconexos."
echo "================================================================"
echo "> 4th test. Size: 10 vertices & 10 edges. 1 repetitions:"
./practica3 1 t4
echo "================================================================"
echo "> 4th test. Size: 10 vertices & 10 edges. 10 repetitions:"
./practica3 10 t4
echo "================================================================"
echo "> 4th test. Size: 10 vertices & 10 edges. 100 repetitions:"
./practica3 100 t4
echo "================================================================"
echo "> 4th test. Size: 10 vertices & 10 edges. 1000 repetitions:"
./practica3 1000 t4
echo "================================================================"
echo "> 4th test. Size: 10 vertices & 10 edges. 10000 repetitions:"
./practica3 10000 t4
echo "================================================================"
echo "> 5st test. Random. 100 repetitions:"
./practica3 100 t5 100 300 120 200 1.0 0.05
echo "================================================================"
echo "> 5st test. Random. 100 repetitions:"
#./practica3 100 t5 1250 1000 500 350 1.0 0.1
echo "================================================================"
echo "> 5st test. Random. 100 repetitions:"
#./practica3 100 t5 10500 12000 1000 500 1.0 0.05