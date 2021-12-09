clear
sudo rmmod arduino
make clean
make
sudo insmod arduino.ko
sudo ./userapp
sudo rmmod arduino
dmesg | tail