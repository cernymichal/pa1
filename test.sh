#!/bin/sh

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

cd ./tests

for in_file in *_in.txt
do
    output=$(../main < "$in_file")
    num=$(echo "$in_file" | sed 's/_in.txt//')
    out_file="${num}_out.txt"
    if ! $(echo "$output" | diff "$out_file" - > /dev/null)
    then
        printf "$FAIL $num\n"
        printf "${WUNDER}Input:${NC}\n$(cat "$in_file")\n${WUNDER}Expected:${NC}\n$(cat "$out_file")\n${WUNDER}Actual:${NC}\n$output\n" | sed "s/^/$INDENT/"
    else
        printf "$SUCCESS $num\n"
    fi
done

