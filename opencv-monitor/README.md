# opencv-monitor
Simple motion monitor using opencv

* install opencv 2.4.*

* complie main 

```
g++ -ggdb `pkg-config --cflags opencv` -std=c++11 -fpermissive -o `basename main` main.cpp `pkg-config --libs opencv`
```

* run main

```
sudo vim /etc/ld.so.conf.d/opencv.conf  
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib  
sudo ldconfig -v
./main -vis -mog1 24 10 out.avi
```
