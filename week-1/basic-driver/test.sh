make clean
make hello.ko

sudo insmod hello.ko
sudo mknod /dev/hello_dev c 500 0 -m 0666

echo "Running tests"

echo "hello" > /dev/hello_dev
cat /dev/hello_dev

sudo rm /dev/hello_dev
sudo rmmod hello