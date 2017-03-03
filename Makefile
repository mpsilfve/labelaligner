TEST_%:%.cc *.cc
	g++ -DTEST_$*_cc -g -o $@ *.cc -std=c++11 -Wfatal-errors

segment:*.cc
	g++ -O3 -g -o $@ *.cc -std=c++11 -Wfatal-errors