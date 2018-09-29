#!/bin/bash

if [ $# -ne 1 ]
then
    echo "need one paramter to excute"
    exit 1
fi

if [ -d $1 ]; then
    echo  `ls $1`
elif [ -x "$1" ]; then		
    echo `./$1`	
elif [ -r $1 ]; then 
    echo  `cat $1`
else
    echo "unkonwn command"
fi