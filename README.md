# LinuxServer 

This project is a lightweight server based on Linux system development, mainly to achieve user registration, 
login verification, file upload and download services......

# Code analysis

Network I/O model：epoll.

Memory management：memory pool of boost.

Threads management:thread pool(https://github.com/progschj/ThreadPool).

Cache database:redis(Asynchronous connection by libevent)

Database system：mariadb(Asynchronous connection pool by libevent)

Disk I/O:libaio.

# Class diagram
![image](https://github.com/HWZ102533/LinuxServer-castration-/blob/main/DFC_CAD/class.png)

# Thread model
![image](https://github.com/HWZ102533/LinuxServer-castration-/blob/main/DFC_CAD/threadModel.PNG)

# Environment set up
1.install centos7 GNU system.

2.install gcc.

yum -y install centos-release-scl

yum -y install devtoolset-9-gcc devtoolset-9-gcc-c++ devtoolset-9-binutils

echo "source /opt/rh/devtoolset-9/enable" >>/etc/profile

3.install clion software

tar -zxvf CLion-2021.1.3.tar.gz

#cd clion-2021.1.3/bin

#./clion.sh

4.install boost library

yum -y install python-devel bzip2-devel zlib-devel  

yum -y install wget

wget https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/boost_1_77_0.tar.gz

tar -zxvf boost_1_77_0.tar.gz

cd boost_1_77_0

./bootstrap.sh

./b2 install

5.install openssl

yum -y install openssl

yum -y install openssl-devel

6.install libevent library

wget https://github.com/libevent/libevent/releases/download/release-2.1.12-stable/libevent-2.1.12-stable.tar.gz --no-check-certificate

tar -xzvf libevent-2.1.12-stable.tar.gz

./configure

make

#make verify 

make install

ln -s /usr/local/lib/libevent-2.1.so.7 /usr/lib64/libevent-2.1.so.7

7.install redis server

wget https://download.redis.io/releases/redis-6.2.6.tar.gz

tar -xzf redis-6.2.6.tar.gz

cd redis-6.2.6

make

8.install mariadb server

yum -y install mariadb mariadb-server

systemctl start mariadb

mysql_secure_installation

mysql -uroot -p < DFC_CAD/all.sql

GRANT ALL PRIVILEGES ON DFC.* TO root@'%' identified by 'password';

9.install libaio

yum -y install libaio.x86_64 libaio-devel.x86_64

10.install cmake

wget https://github.com/Kitware/CMake/releases/download/v3.23.0-rc3/cmake-3.23.0-rc3.tar.gz --no-check-certificate

tar zxvf cmake-3.23.0-rc3.tar.gz

cd cmake-3.23.0-rc3

./bootstrap

gmake 

gmake install

ln -s /usr/local/bin/cmake /usr/bin/cmake

# Running LinuxServer
Now,you can use clion software compile and run the code.

# Contribute
If you find this project helpful, feel free to invite the author for coffee,thank you! 

If you want to obtain the completed version, please contact email:hwz102533@gmail.com.(Tip: Not free)


