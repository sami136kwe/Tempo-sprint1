.PHONY: build clean test

exec = tempo

build:
	$(MAKE) build -C src
	mkdir -p bin
	cp src/$(exec) bin/$(exec)

test: build
	$(MAKE) test -C bats

clean:
	$(MAKE) clean -C src
	rm -rf bin
