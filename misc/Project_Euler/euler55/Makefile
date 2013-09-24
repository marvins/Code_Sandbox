
BIGINT_SOURCES = \
	../utils/BigInteger.cc \
	../utils/BigIntegerUtils.cc \
	../utils/BigUnsigned.cc \
	../utils/BigUnsignedInABase.cc

euler55: euler55.cpp
	g++ -o $@ $< $(BIGINT_SOURCES) -I../utils

clean:
	rm euler55

