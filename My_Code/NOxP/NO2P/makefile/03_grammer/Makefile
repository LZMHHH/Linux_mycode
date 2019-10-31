
A = a b c
B = $(foreach f, $(A), $(f).o)

C =	a b c d/

D = $(filter %/, $(C))
E = $(filter-out %/, $(C))

files = $(wilfcard *.c)

files1 = a.c b.c c.c d.c e.c
files2 = $(wilfcard $(files1))

$(patsubst pattern,replacement,$(var))

all:
	@echo B = $(B)
	@echo D = $(D)
	@echo E = $(E)
	@echo files  = $(files)
	@echo files2 = $(files2)