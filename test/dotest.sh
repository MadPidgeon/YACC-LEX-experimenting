#!/bin/bash

shopt -s nullglob
for d in */; do
	cd $d
	echo "Running tests in $d:"
	for f in *.code 
	do
		name=${f%.code}
		rm -f $name
		../../yolo $f -o $name -O1
		if [ -x $name ]; then
			./$name &> $name.out
			if (($? == 0)); then
				if cmp -s "./$name.out" "./$name.correct"; then
					echo -e "$name: \033[1;32mCorrect\033[0m"
				else
					echo -e "$name: \033[1;31mIncorrect\033[0m"
				fi
			else
				echo -e "$name: \033[1;31mCrash\033[0m"
			fi			
		else
			echo -e "$name: \033[1;31mDoes not compile\033[0m"
		fi
	done
    cd ..
done
