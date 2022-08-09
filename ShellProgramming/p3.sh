#!/bin/bash

if (($# == 1))
then
	directory=$1
else
	directory='./'
fi

files=($(find $directory -name *.log))

for file in ${files[@]}
do
	cat $file | sort -o $file
done

