Program on your computer and send to you Edison via SFTP
===========================

## Why using SFTP ?
I like vi on the intel Edison, but why couldn't I use my lovely sublime text on my computer ? I'll be 100 times efficient
I can !

- Comment the line 
```
    BindToDevice=usb0
``` 
of the file /lib/systemd/system/sshd.socket

- Edit /etc/ssh/sshd_config 
Change the port, choose the one you want 
If you don't have any password on your sessions, check there's a yes at
```
    PermitEmptyPasswords
```

To install SFTP on Sublime text:
```
    Install package manager
    Run the Package Control: Add Repository command from the command palette
    Paste the URL http://sublime.wbond.net/testing/packages.json into the input box at the bottom of Sublime Text and press enter
    Run the Package Control: Upgrade Package command from the command palette and select SFTP
    install package -> SFTP
    Restart Sublime Text
```

To use it, right click on the file/folder in the side bar and upload, download or sync.