CC = gcc
MATAMIKYA_OBJS = matamikya.o matamikya_print.o matamikya_tests.o matamikya_main.o
AMOUNT_OBJS = amount_set_str.o amount_set_str_tests.o amount_set_str_main.o
COMP_FLAG = -std=c99 -Wall -Werror
DEBUG_FLAG = -DNDEBUG

matamikya: $(MATAMIKYA_OBJS) libmtm.a
	$(CC) -c  $(DEBUG_FLAG) $(COMP_FLAG) $*.c
matamikya_print.o: matamikya_print.c matamikya_print.h
	$(CC) -c  $(DEBUG_FLAG) $(COMP_FLAG) $*.c
matamikya_tests.o : ./tests/matamikya_tests.h ./tests/test_utilities.h ./tests/matamikya_tests.c matamikya.h
	$(CC) -c  $(COMP_FLAG) ./tests/$*.c
matamikya_main.o : ./tests/matamikya_main.c ./tests/matamikya_tests.h ./tests/matamikya_tests.c 
	$(CC) -c $(COMP_FLAG) ./tests/$*.c
matamikya.o: matamikya.c matamikya.h  matamikya_print.h
	$(CC) -c  $(DEBUG_FLAG) $(COMP_FLAG) $*.c



amount_set_str: $(AMOUNT_OBJS)
	$(CC) $(AMOUNT_OBJS) -o $@
 amount_set_str.o: amount_set_str.c amount_set_str.h
	$(CC) -c  $(DEBUG_FLAG) $(COMP_FLAG) $*.c
amount_set_str_main.o: amount_set_str.h amount_set_str_main.c ./tests/test_utilities.h
	$(CC) -c  $(DEBUG_FLAG) $(COMP_FLAG) $*.c
amount_set_str_tests.o : ./tests/test_utilities.h  amount_set_str_tests.c amount_set_str.h 
	$(CC) -c  $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(MATAMIKYA_OBJS) $(AMOUNT_OBJS) matamikya amount_set_str