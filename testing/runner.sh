#!/bin/bash

for i in `seq 1 10`; do
	echo $i
	./testing/gen 5 7 5 > tmp/$i.in
	./main < tmp/$i.in > tmp/main$i.out
	tail -n +2 tmp/main$i.out > tmp/main_res$i.out
	./testing/stoer-wagner < tmp/$i.in > tmp/bru$i.out
	diff -b tmp/main_res$i.out tmp/bru$i.out
done