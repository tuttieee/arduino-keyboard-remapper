test:
	@ mkdir -p tests/build \
	&& cd tests/build \
	&& cmake .. \
	&& make \
	&& ctest -V
