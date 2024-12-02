CC=g++
CPPFILES=Encode.cpp Decode.cpp
EXE=Encode.exe Decode.exe

all: $(EXE)

%.exe: %.cpp
	$(CC) $^ -o $@

clean:
	rm -rf $(EXE) *_encoded.bin *_decoded.txt