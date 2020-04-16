#!/bin/bash


extens=(".c" ".cpp" ".h" ".hpp")


line_count=0
funCount(){
    # list all file
    for file in ` ls $1 `
    do
        # if this is a dictionary
        if [ -d $1"/"$file ];then
            funCount $1"/"$file
        else
            fileName=$1"/"$file
            EXTENSION="."${fileName##*.}
            # compare extens array 
            if [[ "${extens[@]/$EXTENSION/}" != "${extens[@]}" ]];then
                declare -i fileLines
                fileLines=`cat $fileName | wc -l`
                echo $fileName" : "$fileLines
                let line_count=$line_count+$fileLines
            fi
        fi
    done
}

if [ $# -ne 1 ];then
    echo "need one dictionary"
else
    funCount $1
fi

echo "linesCount = $line_count" >> ~/codeline.txt