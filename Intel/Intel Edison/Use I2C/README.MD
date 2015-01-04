Use I²C on the Intel Edison 
=========================== 

To use I²C, you first need to try the connectivity

- Create a file called i2c.sh
Put these lines in the file:

```
	echo 28 > /sys/class/gpio/export
	echo 27 > /sys/class/gpio/export
	echo 204 > /sys/class/gpio/export
	echo 205 > /sys/class/gpio/export
	echo 236 > /sys/class/gpio/export
	echo 237 > /sys/class/gpio/export
	echo 14 > /sys/class/gpio/export
	echo 165 > /sys/class/gpio/export
	echo 212 > /sys/class/gpio/export
	echo 213 > /sys/class/gpio/export
	echo 214 > /sys/class/gpio/export
	echo high > /sys/class/gpio/gpio214/direction
	echo high > /sys/class/gpio/gpio204/direction
	echo high > /sys/class/gpio/gpio205/direction
	echo in > /sys/class/gpio/gpio14/direction
	echo in > /sys/class/gpio/gpio165/direction
	echo low > /sys/class/gpio/gpio236/direction
	echo low > /sys/class/gpio/gpio237/direction
	echo in > /sys/class/gpio/gpio212/direction
	echo in > /sys/class/gpio/gpio213/direction
	echo mode1 > /sys/kernel/debug/gpio_debug/gpio28/current_pinmux
	echo mode1 > /sys/kernel/debug/gpio_debug/gpio27/current_pinmux
	echo low > /sys/class/gpio/gpio214/direction
```

- Save and execute typing 
```
	sh i2c.sh
```
- execute these lines :
```
	LD_LIBRARY_PATH=/usr/local/lib
	export LD_LIBRARY_PATH
```
Now, let's try using i2c-tools. It's the linux tool to use i2c in the terminal.
There's a package but it didn't work for most people, so the installation is a bit tricky.

- You should follow this tutorial by Alexander Merz.

Now it's installed, you can test your I²C connectivity by typing this in the terminal:

```
	i2cdetect -y -r 6
```

There should be a result like this, show it has been detected
```
	     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
	00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
	10: -- -- -- -- -- -- -- -- -- 19 -- -- -- -- 1e -- 
	20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	70: -- -- -- -- -- -- -- --  
```

Thanks to Alexander Metz
