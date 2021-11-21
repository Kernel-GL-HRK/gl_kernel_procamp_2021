#!/bin/bash

echo Scissors Game

ROCK_ID=1
PAPER_ID=2
SCISSORS_ID=3

ERROR_ID=-1

ROCK_TITLE="rock"
PAPER_TITLE="paper"
SCISSORS_TITLE="scissors"

function CHOICE_TITLE_WITH_ID {
  case $1 in
     "$ROCK_ID" ) echo "$ROCK_TITLE" ;;
     "$PAPER_ID" ) echo "$PAPER_TITLE" ;;
     "$SCISSORS_ID" ) echo "$SCISSORS_TITLE" ;;
  esac
}

function WIN_CHOICE_WITH_ID {
  case $1 in
     "$ROCK_ID" ) echo "$SCISSORS_ID" ;;
     "$PAPER_ID" ) echo "$ROCK_ID" ;;
     "$SCISSORS_ID" ) echo "$PAPER_ID" ;;
  esac
}

function CHOICE_ID_WITH_MAP_KEY {
case $1 in
    [r,R] ) echo "$ROCK_ID" ;;
    [p,P] ) echo "$PAPER_ID" ;;
    [s,S] ) echo "$SCISSORS_ID" ;;
      *   ) echo "$ERROR_ID" ;;
esac
}

APP_CHOICE="$((1 + $RANDOM % 3))"
APP_CHOICE_TITLE=$(CHOICE_TITLE_WITH_ID "$APP_CHOICE")

TEST_PARAM="test"
if [ "$1" = $TEST_PARAM ]
then 
	echo app choice  "$APP_CHOICE_TITLE"
fi

USER_WIN_CHOICE=$(WIN_CHOICE_WITH_ID "$APP_CHOICE")

USER_CHOICE="$ERROR_ID"
while [ "$USER_CHOICE" = "$ERROR_ID" ] 
  do
	read -p 'Please choose: rock (r) - paper (p) - scissors (s) ' USER_CHOICE_KEY
	USER_CHOICE=$(CHOICE_ID_WITH_MAP_KEY "$USER_CHOICE_KEY")
  done

USER_CHOICE_TITLE=$(CHOICE_TITLE_WITH_ID "$USER_CHOICE")
echo You choose "$USER_CHOICE_TITLE", I choose "$APP_CHOICE_TITLE"

if [ $USER_CHOICE -eq $APP_CHOICE ]
then
	echo Dead hart!
else
	if [ $USER_CHOICE -eq $USER_WIN_CHOICE ] 
	then
 		echo I win: "$APP_CHOICE_TITLE" beats "$USER_CHOICE_TITLE"		
	else
		echo You win: "$USER_CHOICE_TITLE"  beats "$APP_CHOICE_TITLE"
	fi
fi

