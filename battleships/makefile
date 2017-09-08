CC=clang++
LIBS=-std=c++11 -stdlib=libc++
FLAGS=-I../ 

main: 
	$(CC) $(LIBS) $(FLAGS) ../sock_client.cc battleships.cc main.cc -o battleships

server-bot:
	$(CC) $(LIBS) $(FLAGS) -shared battleships.cc server_bot.cc -o server-bot.dll