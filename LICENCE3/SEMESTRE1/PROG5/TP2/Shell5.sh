#!/bin/bash

for num in $*
do
	if let $num 2>/dev/null
	then
		echo "$num : Entier !"
	else
		echo "$num : Non entier..."
	fi
done
