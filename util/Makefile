_::
	@echo run make from repo root

BIEM = ./util/biem

$(BIEM): util/biem.c
	@echo -e "CC\t$@"
	@$(CC) -Ofast -s $< -o $@

clean::
	$(RM) -- $(BIEM)

