#!/bin/sh

FNAME=$1

if [ ! -e $FNAME ]; then
	echo "file $FNAME cannot be found."
	exit -1
fi

PROG_NAME=`echo $FNAME | sed 's/\./_/g'`


rm -f $PROG_NAME".h"
echo -e "const char* $PROG_NAME = " >> $PROG_NAME".h"
while read line
do 
	echo -n -e '"'$line >> $PROG_NAME".h"
	echo "\n"'"' >> $PROG_NAME".h"
done < $FNAME

echo ";" >> $PROG_NAME".h"

