
BIGINT_BASE=../utils

BIGINT_SOURCES= \
	$(BIGINT_BASE)/BigInteger.cc \
	$(BIGINT_BASE)/BigUnsigned.cc \
	$(BIGINT_BASE)/BigUnsignedInABase.cc \
	$(BIGINT_BASE)/BigIntegerUtils.cc \
	$(BIGINT_BASE)/BigIntegerAlgorithms.cc

euler53: euler53.cpp $(BIGINT_SOURCES)
	g++ -o $@ $< $(BIGINT_SOURCES) -O2 -I../utils

clean:
	rm euler53
