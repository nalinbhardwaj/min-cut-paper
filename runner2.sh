#!/bin/bash

echo 1
./testing/gen 100 200 20 > tmp/1.in
time ./main < tmp/1.in
time ./testing/stoer-wagner < tmp/1.in

echo 2
./testing/gen 500 1000 20 > tmp/2.in
time ./main < tmp/2.in
time ./testing/stoer-wagner < tmp/2.in

echo 3
./testing/gen 1000 2000 20 > tmp/3.in
time ./main < tmp/3.in
time ./testing/stoer-wagner < tmp/3.in

echo 4
./testing/gen 2500 5000 20 > tmp/4.in
time ./main < tmp/4.in
time ./testing/stoer-wagner < tmp/4.in

echo 5
./testing/gen 5000 10000 20 > tmp/5.in
time ./main < tmp/5.in
time ./testing/stoer-wagner < tmp/5.in
