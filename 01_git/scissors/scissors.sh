#!/bin/bash

declare -A choice_dict

choice_dict["r"]="rock"
choice_dict["p"]="paper"
choice_dict["s"]="scissors"

key_list=("r" "p" "s")

# Rules
# paper beats rock
# rock beats scissors
# scissors beats paper

while :; do

    computer_choice=${key_list[RANDOM%3]}

    echo
    echo "********************************************************"

    echo -n "computer choice md5sum: "
    echo "$computer_choice" | md5sum | awk '{print $1}'

    # Get input from the player

    echo "Please choose: rock (r) - paper (p) - scissors (s)"
    read -r user_choice
    echo "You choose ${choice_dict[$user_choice]}, I choose ${choice_dict[$computer_choice]}"

    # check who win

    # Combinations when computer wins
    if [[ "$computer_choice" == "p" ]] && [[ "$user_choice" == "r" ]]; then
        echo "I win: paper beats rock"
        break
    elif [[ "$computer_choice" == "r" ]] && [[ "$user_choice" == "s" ]]; then
        echo "I win: rock beats scissors"
        break
    elif [[ "$computer_choice" == "s" ]] && [[ "$user_choice" == "p" ]]; then
        echo "I win: scissors beats paper"
        break

    # Combinations when human wins
    elif [[ "$computer_choice" == "r" ]] && [[ "$user_choice" == "p" ]]; then
        echo "You win: paper beats rock"
        break
    elif [[ "$computer_choice" == "s" ]] && [[ "$user_choice" == "r" ]]; then
        echo "You win: rock beats scissors"
        break
    elif [[ "$computer_choice" == "p" ]] && [[ "$user_choice" == "s" ]]; then
        echo "You win: scissors beats paper"
        break
    else
        echo "We both selected the same, try one more time"
        echo "********************************************************"
        echo
    fi

done
