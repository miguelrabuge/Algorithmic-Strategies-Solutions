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
        [[ -e "$file.out" ]] || continue;
        
        OUT=$("./$EXECDIR/$problem.out" < "$file.in")
        DIFF=$(diff -y "$file.out"  <(echo -e "$OUT"))

        if [ $? -eq 0 ]; then
            echo -e ✅ "${GREEN}TEST PASSED!!${RESET}" "($file)" 
        else
            echo -e ❌ "${RED}TEST FAILED!!${RESET}" "($file)" 
            [[ $SHOW_DIFF == "true" ]] && echo -e "$DIFF"
        fi
    done 
done
