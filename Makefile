WX_CONFIG := wx-config

WX_LIBS := $(shell ${WX_CONFIG} --libs)
WX_CXXFLAGS := $(shell ${WX_CONFIG} --cxxflags)

CXXFLAGS := -std=gnu++2b -Wall -g -D SOURCE_DIRECTORY="$(abspath ${CURDIR})"

INT_DIR := bld
BIN_DIR := bin

OBJECTS := ${INT_DIR}/ChecklistLoader.o ${INT_DIR}/Checklist.o\
               ${INT_DIR}/frames.o ${INT_DIR}/window.o

.PHONY: loader
loader: ${BIN_DIR}/loader

${BIN_DIR}/loader: ${OBJECTS} ${BIN_DIR}
	${CXX} -o $@ ${OBJECTS} ${LDFLAGS} ${WX_LIBS} ${LIBS}
	chmod +x $@

${BIN_DIR}:
	mkdir -p ${BIN_DIR}

${INT_DIR}/%.o: %.cpp ${INT_DIR}
	${CXX} -c -o $@ ${WX_CXXFLAGS} ${CXXFLAGS} $<

${INT_DIR}:
	mkdir -p ${INT_DIR}

.PHONY: clean
clean:
	@rm -f ${BIN_DIR}/loader $(wildcard ${INT_DIR}/*.o)

