pwd
g++ -o main fly/test.cpp -O2 -std=c++17
rm /tmp/pipe.in /tmp/pipe.out
rm data_*
touch /tmp/pipe.in /tmp/pipe.out
