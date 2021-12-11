#!/bin/bash

OLDLIST="/tmp/oldlist"
NEWLIST="/tmp/newlist"

update_list()
{
	ls /dev/ > $1
}

get_diff()
{
	local oldlist_num=`wc -l < $OLDLIST`
	local newlist_num=`wc -l < $NEWLIST`
	local diff=$(($oldlist_num - $newlist_num))
	
	if [ $(($diff + 0)) -eq 0 ]; then
		return 0
	fi
	
	return 1
}

print_diff()
{
	comm -23 <(sort $OLDLIST) <(sort $NEWLIST) | awk '{print $1 " unplugged" }'

	comm -13 <(sort $OLDLIST) <(sort $NEWLIST) | awk '{print $1 " plugged" }'
}



update_list $OLDLIST

while :
do
	sleep 1
	update_list $NEWLIST
	get_diff
	diff=$?
	if [ $(($diff + 0)) -ne 0 ]; then
		print_diff
		cp $NEWLIST $OLDLIST
	fi	
done
