SOURCES = keyreport.cpp tests/keyreport_test.cpp

OBJECTS := $(addsuffix .o, $(addprefix .build/, $(basename $(SOURCES))))
TESTCPPFLAGS = -D_TEST -I. -Itest

.build/%.o: %.cpp
	mkdir -p .build/$(dir $<)
	$(COMPILE.cpp) $(TESTCPPFLAGS) -o $@ $<

runtests: $(OBJECTS)
	$(CC) $(OBJECTS) -lstdc++ -o $@
