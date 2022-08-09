#!/bin/bash

users=($(cat users))

first=0
for user in ${users[@]}
do
	if ((first == 0))
	then
		generated="${generated}${user}@scs.ubbcluj.ro"
		first=1
	else
		generated="${generated}, ${user}@scs.ubbcluj.ro"
	fi
done

echo $generated
