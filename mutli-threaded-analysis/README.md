# mt-collatz

mt-collatz will compute the collatz stopping times for N numbers using T threads where N and T are arguments supplied by the user.

## Getting Started

naviagte to project directory on command line and type:

```
make
```

### Prerequisites

POSIX Implementation

### Usage

mt-collatz N T [-nolock]

N
	the range of numbers to compute on upto 2-100000

T
	the number of threads to create for computation 1-50

-nolock
	do not use critical section mutex locking

## Running the tests

if you have gnuplot installed on your machine you can run the getstats.sh script for a comprehensive automated test and output of graphs

```
./getstats.sh
```

