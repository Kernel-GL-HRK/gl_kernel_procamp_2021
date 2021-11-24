#!/bin/bash

	echo "Game scissors"
	echo "Please doing your choice"
	echo "for choice: rock- press(r), paper- press(p), scissors- press (s)"

move=$1
read moves
if [ "$moves" == r ]; then
	echo "You choose rock, I choose paper"
	echo "I win: paper beats rock"

elif [ "$moves" == p ]; then
	echo "You choose paper, I choose scissors"
	echo "I win: scissors cutting paper"

elif [ "$moves" == s ]; then
	echo "You choose scissors, I choose rock"
	echo "I win: rock break scissors"

else 
	echo you choice = $moves, your choose is loose, try again

fi
