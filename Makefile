CFLAGS = -Wall -std=gnu99 -O0

GIT_HOOKS := .git/hooks/pre-commit

all: $(GIT_HOOKS) main.c
	$(CC) $(CFLAGS) -o main main.c
$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo
clean:
	$(RM) main


