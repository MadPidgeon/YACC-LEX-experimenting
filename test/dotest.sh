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
					echo -e "$name: \033[1;32mCorrect\033[0m"
				else
					echo "$name: \033[1;31Incorrect\033[0m"
				fi
			else
				echo "$name: \033[1;31Crash\033[0m"
			fi			
		else
			echo "$name: \033[1;31Does not compile\033[0m"
		fi
	done
    cd ..
done
