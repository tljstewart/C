#!/bin/bash

threads=(1 2 3 4 5 6 7 8) #16 32)
collatz=(50 250  500 1000 5000 100000)
rp="./results" #interesting, no space between = assignment aloud, if space then interpreted as command
tp="./times"

rm -rf ./results
rm -rf ./times
rm -rf ./graphs
mkdir results
mkdir times
mkdir graphs

make

for i in ${collatz[*]};do
	for j in ${threads[*]};do
		#echo "hi" > file$i.txt;
#		echo "no;" # ; semi-colon doesnt matter apparently
#		echo "yes;";
		#the below line will call the program mt-collatz, it will not advance throuh the for loop until the command has finished. Therefore each command will theroretically have access to the same amount of resources. (not recommended) If you would like to run all the collatz stopping test at once, simply append and ampersand & to the end. Like so,  ./mt-collatz $i $j >> results.csv 2>> times.csv &
		#./mt-collatz $i $j >> $rp/results.csv 2>> $tp/times.csv #all results and times in one file for each test combination, respectively.


		
#no locks
		echo "running range=$i threads=$j nolocks"
		./mt-collatz $i $j -nolock >> $rp/results-$i-$j-nolock.csv 2>> $tp/times-$i-$j-nolock.csv #all results and times in seperate files for each test combination, respectively.  
		printf "\n\n$i-$j-nolock\n" >> $rp/results-total-nolock.csv
		cat $rp/results-$i-$j-nolock.csv >> $rp/results-total-nolock.csv
		cat $tp/times-$i-$j-nolock.csv >> $tp/times-total-nolock.csv
		cat $tp/times-total-nolock.csv | grep ^$i > $tp/times-all-$i-nolock.csv
	
		printf "\n\n$i-$j-nrange-nolock\n" >> $rp/TOTAL-NRANGE-STATS-nolock.csv
		cat $rp/nolock-NRANGE-STATS-$i-$j.csv>> $rp/TOTAL-NRANGE-STATS-nolock.csv	
	
#locks
		echo "running range=$i threads=$j with locks"
		./mt-collatz $i $j >> $rp/results-$i-$j-lock.csv 2>> $tp/times-$i-$j-lock.csv #all results and times in seperate files for each test combination, respectively.  
		printf "\n\n$i-$j-lock\n" >> $rp/results-total-lock.csv
		cat $rp/results-$i-$j-lock.csv >> $rp/results-total-lock.csv
		cat $tp/times-$i-$j-lock.csv >> $tp/times-total-lock.csv
		cat $tp/times-total-lock.csv | grep ^$i > $tp/times-all-$i-lock.csv
	
		printf "\n\n$i-$j-nrange-lock\n" >> $rp/TOTAL-NRANGE-STATS-lock.csv
		cat $rp/lock-NRANGE-STATS-$i-$j.csv>> $rp/TOTAL-NRANGE-STATS-lock.csv



	done;	
	
done

		#make plots, and send to png files
		##using files $tp/times-all*
		printf "\ngenerating threads-vs-time graphs\n"
		gnuplot ./threads-vs-time.pg -persist
		
		##using files $rp/results-total-lock.csv
		printf "\ngenerating histogram graphs\n"
		gnuplot ./histogram.pg -persist

		##using $rp/TOTAL-NRANGE-STATS-*
		printf "\ngenerating race condition graphs\n"
		gnuplot ./racecondition.pg -presist

		##using $rp/resutls/
		printf "\ngenerating a_sub_i value for each n graphs\n"
		gnuplot ./asubi.pg -persist
	
		printf "\nlook in dir./graphs for cool stuff\n"
		ls ./graphs
