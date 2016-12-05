
main: main.c list.c array.c graph.c partition.c weisfeiler-lehman.c
	gcc -std=c99 -o main main.c list.c array.c graph.c partition.c \
	weisfeiler-lehman.c

test: test.c list.c array.c graph.c partition.c weisfeiler-lehman.c
	gcc -std=c99 -o test test.c list.c array.c graph.c partition.c \
	weisfeiler-lehman.c
