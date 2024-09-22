run:
	gcc -g -o esh esh.c
	./esh
clean:
	rm ./esh
.PHONY:
	clean
