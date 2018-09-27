#!/bin/bash

#Dans etc/passwd :
#LOGIN : x : UID : GID : DESCRIPTION : REPERTOIRE_DU_LOGIN : SHELL

for login in $(seq 1 $(wc -l "/etc/passwd" | cut -d' ' -f1))
do
	num=$(cat "/etc/passwd" | cut -d':' -f3 | sed -n "$login"'p'  )
	log=$(cat "/etc/passwd" | cut -d':' -f1 | sed -n "$login"'p')
	if [ $num -gt 1000 ]
	then	
		echo "$log : $num"
	fi
done

