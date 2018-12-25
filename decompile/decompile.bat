@echo off 
cls

xtensa-lx106-elf-objdump -SC %1  > %1.txt
pause 0