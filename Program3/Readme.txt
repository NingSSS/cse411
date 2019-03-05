How to compile in docker:

docker run -it -v /Users/ningsong/Desktop/key-value-server-master:/kvserver cse411 /bin/bash
cd kvserver
##Build
make : compile server；
make client : compile client；
make clean_data:clean data file；
make clean：clean temporary file；
##Run

run ./server

##need other seminal

docker container ls
docker exec -it a6a5 bash //a6a5 is name of container
cd kvserver
run ./client 127.0.0.1 

get [key]            search a key and get the key's value.
put [key]-[value]    put or set a data node(key and value).
del [key]    delete a data node(key and value) into server.
where [regex]    use regex to get value.
save                 save the server's cache data
exit                 kill the client
shutdown   shutdown the server

And I am so sorry that I don't finish the part of reduce. That is so difficult for me. I try to do that but I have nothing...