#!/bin/bash
#set -x

declare -a CHOICES=("" "rock" "paper" "scissors")

test()
{
    if [ $1 -eq $2 ]; then #equal
        return 0
    fi

    if [ $1 -eq 1 ]; then
       if [ $2 -eq 3 ]; then
           return 1 #win
        else
           return 2 #loss
       fi
    fi

    if [ $1 -eq 2 ]; then
        if [ $2 -eq 1 ]; then
           return 1 #win
        else
           return 2 #loss
        fi
    fi

    if [ $1 -eq 3 ]; then
        if [ $2 -eq 2 ]; then
           return 1 #win
        else
           return 2 #loss
        fi
    fi
}

play_game()
{
    #echo parameters: $# , $@
    #lets play this game
    ./my_random.sh
    ret_code=$? #analize ret code

    MD5=`echo -n ${CHOICES[$1]} | md5sum`
    echo "md5sum = $MD5"

    echo you chose ${CHOICES[$1]}
    echo i choice ${CHOICES[$ret_code]}

    test $1 $ret_code
    case $? in
    0)
        echo a draw
        ;;
    1)
        echo a win
        ;;
    2)
        echo a loss
        ;;
    esac

}

echo

PS3='Please choice: rock (r) - paper (p) - scissors (s)'
options=("r" "p" "s" "q")
select opt in "${options[@]}"
do
    case $opt in
        "r")
            play_game 1
            ;;
        "p")
            play_game 2
            ;;
        "s")
            play_game 3
            ;;
        "q")
            break
            ;;
        *) echo "invalid option $REPLY";;
    esac
done
