#!/bin/bash

for i in $*
	do
		echo "Date de dernière modification : "
		ls -d --full-time -lu $i | cut -d' ' -f6,7 | cut -b1-19
		#stat $argument | sed -n '5p'
done


