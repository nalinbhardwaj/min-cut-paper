# min-cut-paper
Implementation of https://arxiv.org/abs/1908.11829

# Usage

- Compile

```bash
g++ -Wall -Werror -Wextra -std=c++11 main.cpp -o main -O2
```

- Run

```bash
./main < input/1.in
```

- Input Format

First, read an undirected weighted graph of the form:

```
n m
u_1 v_1 w_1
u_2 v_2 w_2
.
.
.
u_m v_m w_m
```

where n is the number of vertices, m the number of edges, and w_i is the weight
of the ith edge. Edge weights are read into double-precision floating point
numbers. We assume 0-based indexing for u_i and v_i.

Followed by:

```
d
```

The variable `d` is the exponent in the probability of success 1 − 1/(n)^d for Algorithm 2.