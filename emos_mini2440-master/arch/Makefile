CFLAGS := -I../include -Wall

objs := start.o serial.o s3c2440.o cpuctrl.o


arch.a: $(objs) 
	${AR} -r -o $@ $^
	
%.o:%.c
	${CC} $(CFLAGS) -c -g -o $@ $<
	
%.o:%.s
	${CC} $(CFLAGS) -c -g -o $@ $<
	
clean:
	rm -f arch.a *.o	

