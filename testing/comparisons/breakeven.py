import os

for i in range(1, 20):
    n = i * 100
    m = i * 200
    w = 20000
    print(n, m, w)
    os.system("./testing/gen " + str(n) + " " + str(m) + " " + str(w) + " > tmp/" + str(i) + ".in")
    os.system("time ./main < tmp/" + str(i) + ".in > tmp/main.out")
    os.system("time ./testing/stoer-wagner < tmp/" + str(i) + ".in > tmp/sw.out")
