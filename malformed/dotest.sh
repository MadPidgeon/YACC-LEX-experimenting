#!/bin/bash
shopt -s nullglob
for d in *.code; do
	../lexer $d -p | grep -c "Error" 
done
