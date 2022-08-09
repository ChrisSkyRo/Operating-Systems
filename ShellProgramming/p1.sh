#!/bin/bash

users=($(awk '{print $1}' who.fake))

for user in ${users[@]}
do
	frequency=($(grep -c -e ^$user ps.fake))
	echo $user $frequency
done

