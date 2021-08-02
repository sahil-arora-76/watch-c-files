platform='unkown'
unamestr=`uname`
DIR='build'

if [[ "$unamestr" != 'Linux' ]]; then
    echo "linux required!"
else 
    platform='linux'
fi


if [[ "$platform" == 'linux' ]]; then 
    if [ ! -d "$DIR" ]; then 
        echo "making build dir" 
        mkdir build
        gcc src/main.c -Wall -o build/watch
    else 
        gcc src/main.c -Wall -o build/watch
    fi 
fi