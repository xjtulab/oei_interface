CC=arm-linux-gnueabihf-gcc
OEIHOME = ../oei
OEIARCH = armv7Linux4gcc7.5.0.cortex-a9
SRC = application.c dpde_agent.c HelloWorld.c HelloWorldPlugin.c \
	HelloWorldSupport.c
OBJ = $(patsubst %.c, %.o, $(SRC))
EXEC_OBJ = server.o client.o
CFLGAS = -std=c99 -g
TARGET = single_publisher single_subscriber client server
LDPREFIX = -static-libgcc
LDFLGAS = -L$(OEIHOME)/lib/${OEIARCH} -pthread -ldl -lnsl -lm -lrt \
	-loei_rhsmz -loei_whsmz -loei_discdpdez -loeiz
DEFINES = -DOEI_UNIX -DOEI_LINUX -DOEI_POSIX_THREADS
INCLUDES =  -I. -I$(OEIHOME)/include


all:client server

client:$(OBJ) client.o
	$(CC) $(LDPREFIX) -o $@  $^ $(LDFLGAS)
server:$(OBJ) server.o
	$(CC) $(LDPREFIX) -o $@  $^ $(LDFLGAS)

%.o:%.c
	$(CC) $(CFLGAS) $(INCLUDES) $(DEFINES) -c  $< -o $@



.PHONY: clean
clean:
	@echo "clean all!"
	rm -rf $(OBJ) 
	rm -rf $(TARGET)
	rm -rf $(EXEC_OBJ)
