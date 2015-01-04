Journal fixing
===========================

If there is no room on your Edison, it's because Yocto writes everything in the journal.

The system writes in the journal at each 'reboot'. Using 'shutdown now' does not do this. Described by a FW team member here:
 
Journal logging and rotation are working well, and journald always keep 10% free on root partition (default value).
But when rebooting the board with the "reboot" command, an error occurs at boot time :
systemd-journald[112]: File /var/log/journal/367c1baf2b6d4db9833e6a1d5e4af6a8/system.journal corrupted or uncleanly shut down, renaming and replacing.
The journal is corrupted and an entry is created in /var/log/journal named "xxx.journal~" (around 6Mo).
When performing reboot cycle, root partition becomes full.

# umount /etc/machine-id
# systemd-machine-id-setup
# rm -rf /var/log/journal/*
# reboot
