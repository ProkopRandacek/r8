_::
	@echo run make from repo root

BIEM = ./util/biem

$(BIEM): util/biem.c
	@echo CC $@
	@$(CC) -Ofast -s $< -o $@

clean::
	$(RM) $(BIEM)

