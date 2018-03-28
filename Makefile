TESTDIR=tests
tf=$(wildcard tests/*.cpp)
test_files=$(patsubst tests/%.cpp,tests/%.out,$(tf))

all: $(test_files)

tests/%.out: tests/%.cpp
	$(CXX) -std=c++11 -o $@ $<

test: $(test_files)
	@for x in tests/*.out; do echo "Running test: $$x"; ./$$x; done
	@echo
	@echo "All test passed"
	@echo "Everything is ok"
clean:
	$(RM) $(test_files)

