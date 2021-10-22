#!/bin/bash

<<help

-q - quiet mode
-s - shallow mode
-d=directory - specify directcory with tests (default ./tests)
-x=executable - specify executable to test on (default ./main)

help

NC='\033[0m'
RED_BG='\033[41m'
GREEN_BG='\033[42m'
WBOLD='\033[1;37m'
CBOLD='\033[1;36m'
WUNDER='\033[4;37m'
L_BRACKET="${CBOLD}[${NC}"
R_BRACKET="${CBOLD}]${NC}"
SUCCESS="${L_BRACKET}${WBOLD}${GREEN_BG}V${NC}${R_BRACKET}"
FAIL="${L_BRACKET}${WBOLD}${RED_BG}X${NC}${R_BRACKET}"
INDENT="    "

QUIET=false
SHALLOW=false
TESTS_DIR='./tests'
EXECUTABLE='./main'

while getopts 'qsf:x:' flag
do
    case "${flag}" in
        q)
            QUIET=true
            ;;
        s)
            SHALLOW=true
            ;;
        f)
            TESTS_DIR=${OPTARG}
            ;;
        x)
            EXECUTABLE=${OPTARG}
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            exit 1
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            exit 1
            ;;
    esac
done

EXECUTABLE="$(cd "$(dirname "$EXECUTABLE")" && pwd -P)/$(basename "$EXECUTABLE")"

TESTS_DIRS=( "$TESTS_DIR" )

if ! $SHALLOW
then
    TESTS_DIRS+=( "$TESTS_DIR"/*/ )
fi

for directory in "${TESTS_DIRS[@]}"
do
    if ! $(ls "$directory"/*_in.txt 2>/dev/null >/dev/null)
    then
        continue
    fi

    printf "${WBOLD}$directory${NC}\n"

    for in_file in "$directory"/*_in.txt
    do
        output=$("$EXECUTABLE" < "$in_file")
        num=$(echo "$in_file" | sed 's/_in.txt//')
        out_file="${num}_out.txt"
        num=$(echo "$num" | grep -o '....$')
        if ! $(echo "$output" | diff "$out_file" - > /dev/null)
        then
            printf "$FAIL $num\n"
            
            if ! $QUIET
            then
                printf "${WUNDER}Input:${NC}\n$(cat "$in_file")\n${WUNDER}Expected:${NC}\n$(cat "$out_file")\n${WUNDER}Actual:${NC}\n$output\n" | sed "s/^/$INDENT/"
            fi
        else
            printf "$SUCCESS $num\n"
        fi
    done
done

