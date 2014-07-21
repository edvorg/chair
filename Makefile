all:
	ndk-build
	ant debug install

clean: *
	rm -r obj/
	ant clean
