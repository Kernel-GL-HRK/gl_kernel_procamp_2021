#!/usr/bin/bash
# TODO: Requires bash 4 for associative arrays. Check version

declare -A choices=(['r']='rock' ['p']='paper' ['s']='scissors')
declare -A choice_beats=(['r']='s' ['p']='r' ['s']='p')
# Expanded for clarity
length=${#choices[@]}
keys=("${!choices[@]}")

# Random selection among supported keys
machine_choice=${keys[$(($RANDOM%length))]}
# Generate random magic to hide the machine's choice
# Magic is a random hex number with a constant length
declare -r magic_bytes=10
magic=$(openssl rand -hex $magic_bytes)
# Calculate checksum of magic + machine choice
md5check=$(echo -n $magic $machine_choice | md5sum | awk '{print $1}')

echo '> Please choose: rock (r) - paper (p) - scissors (s)'
echo "> I've made my choice (md5sum: $md5check)"
read -p '> You choose: ' user_choice
# Check that user input is valid
if [ -v ${choices[$user_choice]} ]; then
    echo "> You must select from: ${keys[@]}"; exit 1
fi

# Determine winner
if [ ${choice_beats[$user_choice]} == $machine_choice ]; then
    echo "> You win: ${choices[$user_choice]} beats ${choices[$machine_choice]}"
elif [ ${choice_beats[$machine_choice]} == $user_choice ]; then
    echo "> I win: ${choices[$machine_choice]} beats ${choices[$user_choice]}"
else
    echo "> Tie. We both chose ${choices[$machine_choice]}"
fi

# Show used magic. Magic should be exposed only in the end of the game
echo "> Check me! To compare checksum use:"
echo "echo -n $magic $machine_choice | md5sum | awk '{print \$1}'"
