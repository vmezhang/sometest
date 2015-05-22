#!/bin/bash

COMPILE=pl
RUN=pa

if [ $# -ne 1 ]
then
	exit 2	#参数不对
else
	 $COMPILE $1
	 if [ $? -ne 255 ]
	 then
		# .pc into .bc
		byte_file=`echo $1 | sed "s/\.pc/\.bc/"`	 
		#echo $byte_file
	    $RUN $byte_file
	 else
		 exit 3		#编译失败
 fi
fi
	


