CC = g++
CFLAGS = -g -Wall -O3
LDFLAGS = -lgmp -lmpfr

TARGET = RSA_test

all: $(TARGET)

$(TARGET): $(TARGET).cc
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(TARGET).cc
clean:
	$(RM) $(TARGET)
