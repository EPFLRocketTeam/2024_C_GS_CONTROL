<p align="center">
    <img width="200" src="https://github.com/LaGSdeKarlHeinz/Control/actions/workflows/main.yml/badge.svg" alt="Test bage">
</p>

# Installation 
First of all you need to pull all the submodules with the following command:
```shell
git submodule update --init --recursive
```

In order to compile the 2 parts of the software you need to install qt6 base dev, on ubuntu it gives the following: 
```shell
sudo apt install qt6-base-dev
```

Then some other dependencies required need to be installed:
```shell
sudo apt install libqt6svg6-dev libqt6serialport6-dev qtbase5-dev-tools libboost-all-dev libgtest-dev qt6-base-dev-tools

```
Then you must compile the resources by going:
```shell
rcc -binary GUI/res/resources.qrc -o GUI/res/resources.rcc
```
After this you can install create a build directory at the root of the project: 
```shell
mkdir build && cd build
```
And build all the targets:
```shell 
cmake .. && make
```

Now you should create the 2 following files:
- in /Server/src create a auth_key.json file with for example the following content:
```json 
[
    {
        "key": "ClientSecretKey1",
        "accessRight": 3
    },
    {
        "key": "ClientSecretKey2",
        "accessRight": 3
    }
]
```
- in /GUI/src create a .key file that just contain one of the keys setup above. for example just "ClientSecretKey1" 

Then you should executre both the server and the GUI buy running
```shell 
./FirehornServer
```
And in another terminal window
```shell
./FirehornGUI
```

QTSvg : sudo apt-get -y install libqt6svg6-dev

Rcc command : fedora : sudo dnf install qt-devel

SerialPort : ubuntu : sudo apt-get install libqt5serialport5-dev

Boost : ubuntu : sudo apt install libboost-all-dev

install gtest : sudo apt-get install libgtest-dev


# Communication structure 

```
communication from client to server :
{
    header: post | get | internal,
    payload: {
        source : <client-ip>,
        args*...
    }
}

communication from sever to client :
{
    header: post | get | internal,
    payload: {
        source : GSE | AV,
        args*...
    }
}
```
get :                                        
    getSerialStatus => response = "serialNameUsed", "serialStatus" (fields expected)


# TODO

- Server respond and make command from clients
- Stress tests
- authentication
- multiple concurrent input handler
- separate data handling in a different thread  

- Mettre le AV data pannels sur des cards 
