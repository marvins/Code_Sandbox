
CFLAGS=-O3

BIGINT_SOURCES= \
	../utils/BigInteger.cc \
	../utils/BigIntegerUtils.cc \
	../utils/BigUnsigned.cc \
	../utils/BigUnsignedInABase.cc \
	../utils/BigIntegerAlgorithms.cc

euler61: euler61.cpp
	g++ $< -o $@ -I../utils $(CFLAGS) $(BIGINT_SOURCES)

clean: 
	rm euler61
