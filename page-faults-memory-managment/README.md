# matrix

matrix will compute the average time it takes to read and array by rows and columns, it will also calculate the average time it takes to write an array by rows and columns. Where the size of a row is intentionally the of a system page 4096 bytes.

## Getting Started

naviagte to project directory on command line and type:

```
make
```

### Prerequisites

POSIX Implementation

### Usage

./matrix

or


## Running the tests

if you have gnuplot installed on your machine you can run the time.pg script for a comprehensive automated test and output of the times_graph.pg, below is the command to run

```
./matrix > time.csv 2>alltimes.csv && ./time.pg
```

