#!/bin/bash

shopt -s nullglob
for d in */; do
	cd $d
	echo "Running tests in $d:"
	for f in *.code 
	do
		name=${f%.code}
		../../lexer $f -o $name -O0
		if [ -x $name ]; then
			./$name &> $name.out
			if (($? == 0)); then
				if cmp -s "./$name.out" "./$name.correct"; then
					echo "$name: Correct"
				else
					echo "$name: Incorrect"
				fi
			else
				echo "$name: Crash"
			fi			
		else
			echo "$name: Does not compile"
		fi
	done
    cd ..
done
