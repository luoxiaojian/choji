#!/bin/bash
expHome=~/workspace/choji/
tasksetDir=$expHome/tasksets/chojies/
targetDir=$expHome/exp/result/

for dirname in $(ls $tasksetDir)
do
	for tsname in $(ls $tasksetDir/${dirname}/* )
	do
		./choji $tsname >> $targetDir/$dirname
		if [ $? = 0 ]; then
			echo
		else
			echo $tsname "error"
		fi
	done
done

#	./demo ./tasksets/$tsname 
