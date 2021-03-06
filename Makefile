CXXFLAGS = -std=c++11 -O3 -g -Wall -pedantic
#-g -Wall -pedantic

PRG = TEAscus
GIT_HEADER = include/gitversion.h
CPP_FILES = $(wildcard src/[A-Z]*.cpp)
CPP_TEST_CLASSES = $(wildcard test/[A-Z]*.cpp)
# TEST_FILES = $(wildcard test/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(CPP_FILES))
OBJ_TESTS = $(patsubst test/%.cpp, build/%.o, $(CPP_TEST_CLASSES))
ifndef INSTAL_PREFIX
    INSTAL_PREFIX = /usr/local
endif

# DEFAULT = classical build
.PHONY : all
all : build $(PRG)

# TEST
.PHONY : test
test : build TEAscus_test
	./TEAscus_test && rm TEAscus_test

# INSTALL
.PHONY : install
install : $(PRG)
	install -C $< $(INSTAL_PREFIX)/bin

# CLEANING
.PHONY : clean
clean :
	-rm -r $(PRG) build

# BUILDING
$(PRG) : build/main.o $(GIT_HEADER) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $< $(OBJ)

build/%.o : src/%.cpp $(GIT_HEADER)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

build/%.o : test/%.cpp $(GIT_HEADER)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

build :
	mkdir $@

# .git/COMMIT_EDITMSG is a file that get updated with every commit
# .git/HEAD is a file that get updated with every switch of branches
# if any of those have changed, commit in binary have to be updated
$(GIT_HEADER) : ../.git/modules/TEAscus/HEAD ../.git/modules/TEAscus/COMMIT_EDITMSG
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > $@

# if repository is cloned, it does not contain last commit message
# to make a git header regarless if absence of the last commit message
# we create a blank file with last commit message
../.git/modules/TEAscus/COMMIT_EDITMSG :
	touch $@

# TESTING binary

TEAscus_test : build/TEAscus_test.o $(OBJ) $(OBJ_TESTS)
	gcc -o $@ $^ -lstdc++ -lcppunit -ldl -lm
