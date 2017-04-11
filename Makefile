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

clean:
	$(RM) $(EXEC)


