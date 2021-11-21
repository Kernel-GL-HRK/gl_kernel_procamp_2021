#!/usr/bin/env bash

WEAPON_ROCK="rock"
WEAPON_PAPER="paper"
WEAPON_SCISSORS="scissors"

read_user_choice() {
    local user_choice

    read -r -N 1 user_choice
    case "${user_choice}" in
    "${WEAPON_ROCK:0:1}")
        user_choice="${WEAPON_ROCK}"
        ;;
    "${WEAPON_PAPER:0:1}")
        user_choice="${WEAPON_PAPER}"
        ;;
    "${WEAPON_SCISSORS:0:1}")
        user_choice="${WEAPON_SCISSORS}"
        ;;
    *)
        exit 1
        ;;
    esac

    echo "${user_choice}"
}

pc_choice() {
    local pc_choice

    case $((RANDOM%3)) in
    0)
        pc_choice="${WEAPON_ROCK}"
        ;;
    1)
        pc_choice="${WEAPON_PAPER}"
        ;;
    2)
        pc_choice="${WEAPON_SCISSORS}"
        ;;
    *)
        echo "ERROR: Unexpected choice!"
        exit 1
        ;;
    esac

    echo "${pc_choice}"
}

fight() {
    local user="${1}"
    local pc="${2}"
    local ret_code

    case "${user}" in
    "${WEAPON_ROCK}")
        if [[ "${pc}" == "${WEAPON_SCISSORS}" ]] ; then
            # User win
            ret_code=1
        elif [[ "${pc}" == "${WEAPON_ROCK}" ]] ; then
            # dead heat
            ret_code=0
        else
            # PC win
            ret_code=2
        fi
        ;;
    "${WEAPON_PAPER}")
        if [[ "${pc}" == "${WEAPON_ROCK}" ]] ; then
            # User win
            ret_code=1
        elif [[ "${pc}" == "${WEAPON_PAPER}" ]] ; then
            # dead heat
            ret_code=0
        else
            # PC win
            ret_code=2
        fi
        ;;
    "${WEAPON_SCISSORS}")
        if [[ "${pc}" == "${WEAPON_PAPER}" ]] ; then
            # User win
            ret_code=1
        elif [[ "${pc}" == "${WEAPON_SCISSORS}" ]] ; then
            # dead heat
            ret_code=0
        else
            # PC win
            ret_code=2
        fi
        ;;
    esac

    return ${ret_code}
}

# Game flow
echo "Please choose: ${WEAPON_ROCK} (${WEAPON_ROCK:0:1}) - ${WEAPON_PAPER} (${WEAPON_PAPER:0:1}) - ${WEAPON_SCISSORS} (${WEAPON_SCISSORS:0:1})"
my_choice=$(pc_choice)
user_choice=$(read_user_choice) || {
    echo ""
    echo "You choose illegal weapon!"
    exit 1
    }
echo ""
echo "You choose ${user_choice}, I choose ${my_choice}"
fight "${user_choice}" "${my_choice}"
case ${?} in
0)
    echo "Dead heat: ${user_choice} and ${my_choice}"
    ;;
1)
    echo "You win: ${user_choice} beats ${my_choice}"
    ;;
2)
    echo "I win: ${my_choice} beats ${user_choice}"
    ;;
*)
    echo "ERROR: Unexpected result!"
    exit 1
    ;;
esac
