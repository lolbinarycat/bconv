</$objtype/mkfile	

binpath=/$objtype/bin
manpath=/sys/man

build:V: bconv

install:V: build
	cp bconv.man $manpath/1/bconv
	cp bconv $binpath/bconv

clean:V:
	rm -f bconv bconv.6

&: &.c
	$CC $stem.c
	$LD -o $stem $stem.$O
