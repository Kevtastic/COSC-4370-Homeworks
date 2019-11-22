g++ -c *.cpp `libpng-config --cflags`
g++ -o extra *.o `libpng-config --ldflags --libs`
rm *.o