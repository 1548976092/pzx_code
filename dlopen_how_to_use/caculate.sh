#编译参数 gcc -fPIC -shared
gcc -fPIC -shared caculate.c -o libcaculate.so 
gcc -rdynamic -o main main.c -ldl


