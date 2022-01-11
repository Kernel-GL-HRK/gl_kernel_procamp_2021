#!/bin/bash

arr=( rock paper scissors )

RANGE=3
number=$RANDOM
let "number %= $RANGE"


echo "Please choose: rock (r) - paper (p) - scissors (s)"
read -p "your choice: " choice


if [ $choice == r ]; then
    choice=0
elif [ $choice == p ]; then
    choice=1   
elif [ $choice == s ]; then
    choice=2 
fi



echo You choose ${arr[$choice]}, I choose ${arr[$number]}

if [ $choice -eq $number ]; then
    echo draw!!!
elif [ $choice -eq 0 ]; then
    if [ $number -eq 1 ]; then
        echo You win: ${arr[$number]} beats ${arr[$choice]}
    else
        echo I win: ${arr[$choice]} beats ${arr[$number]}
    fi
elif [ $choice -eq 1 ]; then    #paper 1
    if [ $number -eq 0 ]; then  #rock  0
        echo You win: ${arr[$choice]} beats ${arr[$number]}
    else
        echo I win: ${arr[$number]} beats ${arr[$choice]}
    fi
elif [ $choice -eq 2 ]; then
    if [ $number -eq 0 ]; then
        echo I win: ${arr[$number]} beats ${arr[$choice]}
    else
        echo You win: ${arr[$choice]} beats ${arr[$number]}
    fi
fi


# -lt less

