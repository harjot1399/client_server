all: client server man

man: client.man server.man
	groff -Tpdf -man client.man > client.pdf
	groff -Tpdf -man server.man > server.pdf

client: client.cpp 
	g++ -O client.cpp -o client

server: server.cpp 
	g++ -O server.cpp -o server

clean:
	rm -f client server client.pdf server.pdf