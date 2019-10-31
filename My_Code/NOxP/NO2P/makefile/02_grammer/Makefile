
mtest:a.o b.o c.o
	gcc -o mtest $^
	
%.o: %.c
	gcc -c -o $@ $<
	
clean:
	rm *.o mtest
	
.PHONY:clean
	
