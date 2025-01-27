savedcmd_/home/wilson/Desktop/lab_1/simple.mod := printf '%s\n'   simple.o | awk '!x[$$0]++ { print("/home/wilson/Desktop/lab_1/"$$0) }' > /home/wilson/Desktop/lab_1/simple.mod
