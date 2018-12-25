# costycncwifiname
How change text directly in bin file

You can change some text in bin file directly.

For example ... I create and compile with Arduino for esp8255 a program with can command

the costycnc machine http://www.costycnc.it under wifi

But name of wifi module, name of wifi modem and password is for mine modem . so...

I learn how change these with yours...

Change name of ip station
------------------------
Name wifi of this ip module is "Costycnc"

You can change quickly this name in this mode:

Download hexviewer from here https://mh-nexus.de/en/downloads.php?product=HxD20

open with this editor file costycnc.bin

Search Costycnc or go to at address 00050AB0 and

change Costycnc with your name

Change com3 with your port in upload.bat

save and run upload.bat

at address 00050E70 is password and wifi name that need change to connect with your wifi station

change Infostrada-2.4GHz-02C3F6 with your name home modem wifi

and change costycnc1967 with your password

Your text need to finish with 0

00050AB0 Costycnc 0 0 0 Connecting to %s

Folder decompile contain decompiler and files compiled by Arduino
