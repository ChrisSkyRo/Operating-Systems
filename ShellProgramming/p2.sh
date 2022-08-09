#!/bin/bash

if (($# == 1))
then
	directory=$1
else
	directory='./'
fi

files=($(find $directory -name *.c | tac))

count=0
for file in ${files[@]}
do
	size=$(wc -l $file | cut -d ' ' -f 1)
	if (($size > 500))
	then
		((count++))
		echo $file
		if ((count == 2))
		then
			exit 1
		fi
	fi
done

