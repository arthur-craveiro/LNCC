export PATH=$PATH:./
#!/bin/bash
gcc -c minhasFuncoes.c
gcc -c alomundo.c
gcc *.o -o alomundo.exe
