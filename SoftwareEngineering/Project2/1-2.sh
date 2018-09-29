#!/bin/bash

# using function
print() {
    echo "***************************"
    echo "*          MENU           *"
    echo "*  1.copy       2.rename  *"
    echo "*  3.remove     4.find    *"
    echo "*  5.exit                 *"
    echo "***************************"
}

print
read  num
if [ $num = 1 ]; then
    echo "enter source and aim file to copy:"
    read source_file aim_file
    cp $source_file  $aim_file
elif [ $num = 2 ]; then
    echo "enter rename file and new name:"
    read file_name new_name
    mv $file_name  $new_name
elif [ $num = 3 ]; then
    echo "file name to delete:"
    read file_name 
    rm $file_name
elif [ $num = 4 ]; then
    echo "file name to find:"
    read file_name
    find $file_name
elif [ $num = 5 ]; then
    exit 1
else
    echo "unkonwn command"
fi

