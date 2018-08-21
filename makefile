TARGETS = main ls	

.PHONY: clean

all: $(TARGETS)

%: %.c
	gcc $< -o $@

clean:
	rm -f $(TARGETS)
