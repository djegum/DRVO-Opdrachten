make clean
make hello.ko

sudo insmod hello.ko
sudo mknod /dev/hello_dev c 500 0 -m 0666

echo "Running tests"

echo "hello" > /dev/hello_dev
cat /dev/hello_dev

echo "Running C tests"

gcc test.c -o test.o

./test.o

sudo rm /dev/hello_dev
sudo rmmod hello