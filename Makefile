CFLAGS = -Wall -std=gnu99 -O0
EXEC = naive sub_matrix

GIT_HOOKS := .git/hooks/pre-commit

all: $(GIT_HOOKS) $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo
SRCS_common = main.c impl.c

naive: $(SRCS_common)
	$(CC) $(CFLAGS) -D$@ -o $@ main.c
sub_matrix: $(SRCS_common)
	$(CC) $(CFLAGS) -D$@ -o $@ main.c
cache-test: $(EXEC)
	echo 1 | sudo tee /proc/sys/vm/drop_caches && \
		perf stat --repeat 2 -e cache-misses,cache-references,instructions,cycles \
		./naive
	echo 2 | sudo tee /proc/sys/vm/drop_caches && \
		perf stat --repeat 2 -e cache-misses,cache-references,instructions,cycles \
		./sub_matrix
clean:
	$(RM) $(EXEC)


