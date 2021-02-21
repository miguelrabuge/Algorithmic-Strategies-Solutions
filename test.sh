#!/bin/bash

SHOW_DIFF="true"

TESTDIR="./tests"
[[ -d "$EXECDIR" ]] && exit 1

PROBLEMS="A B C"
[ $# -ne 0 ] && PROBLEMS=$*

EXECDIR="./bin"
make $PROBLEMS -s || exit 1;

for problem in $PROBLEMS; do
    echo "## RUNNING TESTS FOR PROBLEM $problem..."
    for f in "$TESTDIR/$problem/"*.in; do 
        file=${f%.*}
        [[ -e "$file.out" ]] || break

        DIFFOUT=$(diff -y "$file.out"  <("./$EXECDIR/$problem.out" < "$file.in"))
    
        if [ -z "$DIFFOUT" ]; then
            echo -e ✅ "${GREEN}TEST PASSED!!${RESET}" "($file)" 
        else
            echo -e ❌ "${RED}TEST FAILED!!${RESET}" "($file)" 
            [[ $SHOW_DIFF == "true" ]] && echo -e "$DIFFOUT"
        fi
    done 
done
