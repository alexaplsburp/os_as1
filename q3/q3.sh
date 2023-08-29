#!/bin/bash
if [ ! -d "Result" ] ; then
        mkdir Result

fi
touch output.txt
cd Result
while IFS= read -r line;
do
    IFS=" " read -ra data <<< "$line"
    read x y z <<< "$line"
    echo "$line"
    data_new="$(echo -e "${data[2]}" | tr -d '[:space:]')"


    if [ "$data_new" == "xor" ]; then
            echo "$(( x^y ))" >> output.txt
    elif [ "$data_new" == "product" ] ; then
            echo "$(( x*y ))" >> output.txt
    else
        if [ "$x" -gt "$y" ] ; then
                echo "$x" >> output.txt
        elif [ "$y" -gt "$x" ] ; then
                echo "$y" >> output.txt
        else
                echo "number are equal"
        fi
    fi

done < input.txt
