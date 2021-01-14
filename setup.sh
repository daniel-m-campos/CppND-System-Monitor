sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install gcc-snapshot
sudo apt-get install gcc-9 g++-9

export CC=/usr/bin/gcc-9
export CXX=/usr/bin/g++-9