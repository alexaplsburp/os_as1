#!/bin/bash

while IFS= read -r line
do
    read a b op <<< "$line"
    if [ "$op" = "product" ]; then

            echo "$((a ^ b))" > output.txt
    fi
done < data3.txt
