# CS551

## ⚙️ Lab 0

Run the following command in the terminal:
```
./test.sh
```

If you got permission denied, first run this and then try running './test.sh' command:
```
chmod 777 test.sh
```

The test.sh file simply generates the ppmcvt executable file using 'make' command (GCC and CMake must be installed on the system) and then runs 12 tests using a command like this: 

```
./ppmcvt -flag1 -flag2 output-image-name.pbm input-image-name.ppm
```
You can simply use 'make' command (This command uses the contents of the Makefile) to generate the executable file and manually test if your program works.
