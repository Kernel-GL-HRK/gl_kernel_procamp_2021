#!/bin/bash
#set -x

# возвращает число от MINVALUE до MAXVALUE

#define valrs
MAXVALUE=3
MINVALUE=1
RET=0

#check params count
if  [[ "$#" -eq 1 ]]; then
    MAXVALUE=$1
    echo RECV MAX VALUE $MAXVALUE
fi

# randomize function, return random value
RANDOMIZER()
{
    let RET=$((RANDOM % 10))
    #echo $RET
    return $RET
}


#try find value > MINVALUE and value < MAXVALUE and then break circle
while [ 1 ]; do
    #let RET=$((RANDOM % 10))
    RANDOMIZER
    RET=$?
    #echo "Compare $RET ( $MINVALUE $MAXVALUE)"
    if [[ ($RET -ge $MINVALUE) && ($RET -le $MAXVALUE) ]]; then
	#echo $RET
	break
    fi
done

#return value
exit $RET
