all: wav_in_mem

wav_header.h: tone.wav
	xxd -i tone.wav wav_file.h

wav_in_mem: wav_header.h
	g++ wav_in_mem.cpp -o wav_in_mem

clean: 
	rm wav_in_mem

