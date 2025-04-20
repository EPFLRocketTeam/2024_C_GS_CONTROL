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
If not already installed on ubuntu system you might need a compiler and SQLite3 :
```shell
sudo apt install g++ build-essential
sudo apt install libsqlite3-dev
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

# Config files
There are 2 config files:
- GUI/src/Setup.h 
- Server/src/Setup.h 

In those 2 files the main thing you can change is whether you want the Firehorn version of the program of the Icarus version. 
However in the server setup there are 2 additional fields you can change:
- SIMULATE_PACKETS: can be set to true or false and is an option to enable debug packets that are generated randomly and send to the GUI every second
- DEBUG_LOG: that can be set to true or false and change de debug level to print the debug log in the terminal output

# Random library references (Not relevant)
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
