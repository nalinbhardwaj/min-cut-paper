#!/bin/bash

for i in `seq 1 20`; do
	n = expr $i \* 100
	m = expr $i \* 200
	w = 20
	./testing/gen $n $m $w > tmp/$i.in
	time ./main < tmp/$i.in > tmp/main.out
	time ./testing/stoer-wagner < tmp/$i.in > tmp/sw.out
done
