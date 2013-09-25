
BIGINT_SOURCES= \
	../utils/BigInteger.cc \
	../utils/BigIntegerAlgorithms.cc \
	../utils/BigUnsigned.cc \
	../utils/BigUnsignedInABase.cc \
	../utils/BigIntegerUtils.cc


euler56: euler56.cpp
	g++ -o $@ $< $(BIGINT_SOURCES) -I../utils


clean: 
	rm euler56

