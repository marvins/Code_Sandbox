
BIGINT_BASE=..

BIGINT_SOURCE= \
	$(BIGINT_BASE)/BigIntegerUtils.cc \
	$(BIGINT_BASE)/BigUnsignedInABase.cc \
	$(BIGINT_BASE)/BigUnsigned.cc \
	$(BIGINT_BASE)/BigIntegerAlgorithms.cc \
	$(BIGINT_BASE)/BigInteger.cc

utility_tests: utility_tests.cpp
	g++ -o $@ $< $(BIGINT_SOURCE) -I..

clean:
	rm utility_tests

