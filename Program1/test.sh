TARGET=./obj64/gauss.exe

for s in 256 512 1024 2048 4096 8192
do
    echo "Running at size=" $s
    for i in `seq 5`
    do
        $TARGET -n $s
    done
done

# TODO: you will want to make the testing more rigorous, for example by allowing
#       to change the number of threads