CC=gcc

#User settings
GLIBFLAGS=`/usr/bin/pkg-config --cflags glib-2.0`
CLANGFLAGS=-I./src/include/clang-c
GLIBLIB=`/usr/bin/pkg-config --libs glib-2.0`
CLANGLIB= -lclang `/usr/bin/llvm-config --ldflags`

CFLAGS= $(GLIBFLAGS)  $(CLANGFLAGS)  -std=c99 -c -I./src/include
CLIBS= $(CLANGLIB) $(GLIBLIB)

SOURCES=src/*.c

OBJECTS= App.o ASTNode.o ASTNodeUtil.o ASTPrinter.o ASTUnit.o Milu3.o MultiProcessing.o Mutant.o MutantExecutor.o MutantSet.o MutationId.o MutationOperator.o MutationTemplate.o Mutator.o Options.o Project.o TestInput.o Utility.o $(OPOBJ) $(MUTOBJ)

MUTOBJ=add_arithmetic_assignment_replacement.o add_arithmetic_replacement.o and_logical_negation.o and_logical_replacement.o austin_add_arithmetic_replacement.o austin_and_logical_negation.o austin_and_logical_replacement.o austin_divide_arithmetic_replacement.o austin_equal_relational_replacement.o austin_greater_equal_relational_replacement.o austin_greater_relationa_replacement.o austin_integer_constant_replacement.o austin_less_equal_relational_replacement.o austin_less_relational_replacement.o austin_modulus_arithmetic_replacement.o austin_multiply_arithmetic_replacement.o austin_not_equal_relational_replacement.o austin_or_logical_negation.o austin_or_logical_replacement.o austin_subtract_arithmetic_replacement.o bitwise_and_logical_assignment_replacement.o bitwise_and_logical_replacement.o bitwise_or_logical_assignment_replacement.o bitwise_or_logical_replacement.o divide_arithmetic_assignment_replacement.o divide_arithmetic_replacement.o equal_relational_replacement.o greater_equal_relational_replacement.o greater_relational_replacement.o if_expression_negation_replacement.o integer_constant_replacement.o less_equal_relational_replacement.o less_relational_replacement.o modulus_arithmetic_assignment_replacement.o modulus_arithmetic_replacement.o multiply_arithmetic_assignment_replacement.o multiply_arithmetic_replacement.o not_equal_relational_replacement.o or_logical_negation.o or_logical_replacement.o postfix_decrement_replacement.o postfix_increment_replacement.o prefix_decrement_replacement.o prefix_increment_replacement.o subtract_arithmetic_assignment_replacement.o subtract_arithmetic_replacement.o break_continue_replacement.o float_constant_replacement.o statement_deletion.o integer_var_abs_insertion.o float_var_abs_insertion.o integer_var_uoi_insertion.o while_expression_negation_replacement.o remove_null_assignment.o replace_calloc_with_null.o replace_malloc_with_null.o replace_malloc_size_with_zero.o replace_calloc_size_with_zero.o replace_sizeof_argument_with_pointer_equivalent.o replace_sizeof_argument_with_nonpointer_equivalent.o

OPOBJ=Austin_CRCR.o Austin_OAAN.o Austin_OLLN.o Austin_OLNG.o Austin_ORRN.o CRCR.o ABS.o OAAA.o OAAN.o OBBA.o OBBN.o OCNG.o OIDO.o OLLN.o OLNG.o ORRN.o SBRC.o SSDL.o UOI.o RMNA.o REDAWN.o REDAWZ.o RESOTPE.o REMSOTP.o

all : init milu clean 

milu : $(OBJECTS) 
	$(CC) $(OBJECTS) -o bin/milu -O2 $(CLIBS) 

%.o : src/%.c 
	$(CC) $(CFLAGS) $< -o $@

%.o : src/mutation/%.c 
	$(CC) $(CFLAGS) $< -o $@

%.o : src/operators/%.c
	$(CC) $(CFLAGS) $< -o $@

%.o : src/mutators/selective_c/*/%.c
	$(CC) $(CFLAGS) $< -o $@

%.o : src/mutators/austin/*/%.c
	$(CC) $(CFLAGS) $< -o $@

%.o : src/mutators/other_c/*/%.c
	$(CC) $(CFLAGS) $< -o $@
	
%.o : src/mutators/memory_c/*/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	chmod +x bin/milu
	rm -rf *o
init:
	mkdir -p bin
