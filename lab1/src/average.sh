#!/bin/sh
exec 0<./numbers.txt
echo There were $# parameters passed.
for param in "$@"
do
sum=$(($sum+$param))
count=$(( $count + 1 ))
done
echo Averagge is $(($sum/$count))
