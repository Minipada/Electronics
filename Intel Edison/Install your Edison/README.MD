Install Intel Edison
===================

Just a tutorial to install everything you could need on the Intel Edison.

------
### Only works on Windows

1. Plug the 12V in
2. Plug in the USB, the **right** one (near the 12V power)
3. Check the **J16** switch is at **left**
4. Download the latest image (https://communities.intel.com/docs/DOC-23242), take the **Edison Yocto complete image**
5. Format the Edison disk in **FAT32** (if you don't see it on your computer, check the USB cable is at right)
6. Paste everything from the **Yocto image** (atm it is *edison-image-rel1-maint-rel1-ww42-14.zip*)
7. **Unplug** the USB and plug the left one
8. Go to **Putty**, connect to your board (find the COM, 115200 baudrate)
9. The login is **root**, there is no password
10. Type in :
```shell
reboot ota
```
It will reboot your Edison board and **install the new firmware**, which is less buggy. This one has also a command to upgrade itself. It can take 3 minutes.
11. Login again, **check** the new version is installed, type:
```shell
cat /etc/version
```
The output should be this atm:
**_edison-rel1-maint-weekly_build_16_2014-10-14_14-56-19_**
12. Let's configure the board:
```shell
configure_edison --setup
```
Follow the instructions (name your device and connect to your Wifi)
13. Test the ping
```shell
ping google.com
```
You should have something like this:
**_64 bytes from 64.233.166.139: seq=0 ttl=40 time=37.932 ms_**
If not, you failed something man

14.Next, you'll **change the repository** installed by default on the Edison to have the new ones !
``` shell
vi /etc/opkg/base-feeds.conf
```
Type **i** to edit a file in the vi editor
Delete everything and copy past this:
``` shell
src/gz all http://repo.opkg.net/edison/repo/all
src/gz edison http://repo.opkg.net/edison/repo/edison
src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32
```
15.Type in
``` shell
opkg update
```
16.Type all this commands, if you want them, I think you'll use them someday:
``` shell
opkg install git
opkg install cmake
opkg install wget
opkg install apache2
opkg install i2c-tools
```
17.Type in to download pip 
``` shell
wget --no-check-certificate https://bootstrap.pypa.io/get-pip.py
```
18.Type in to install pip 
``` shell
python get-pip.py
```
19.Type in to install pyserial
``` shell
pip install pyserial
```
20.Test the mraa library:
``` shell
vi blink.py
```

``` python
import mraa
import time

x = mraa.Gpio(13)
x.dir(mraa.DIR_OUT)

while True:
    x.write(1)
    time.sleep(0.2)
    x.write(0)
    time.sleep(0.2)
```

To leave the vi editor, type **:wq** and hit **Enter**

```shell
python blink.py
```
**You should see a led blinking on your board !**
**You now have your Yocto completely installed, Bravo !**

21.You can dance !

![I only came for dancing](https://38.media.tumblr.com/d7437511768f006890aa4af9d564ef13/tumblr_n46jgnVMt41savhp7o3_400.gif)

22. To shutdown your Edison, don't unplug it like an uncivilized man (or woman), type :

``` shell
halt
```
You'll see much lines, the last one will be:
**[  OK  ] Reached target Shutdown.**

Now you can unplug everything