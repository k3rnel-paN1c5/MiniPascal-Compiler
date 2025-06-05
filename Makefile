# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build
TESTDIR := tests

# Compiler and Flags
CXX := g++
CXXFLAGS := -std=c++17 -I$(INCDIR) -I$(BUILDDIR)
LDFLAGS :=
LDLIBS :=

# Executable Name
TARGET := compiler

# Source Files
SRCS := $(wildcard $(SRCDIR)/*.cpp)
LEX_SRC := $(SRCDIR)/scanner.lex
YACC_SRC := $(SRCDIR)/parser.y

# Generated Files 
PARSER_H := $(BUILDDIR)/parser.h
PARSER_CPP := $(BUILDDIR)/parser.cpp
LEX_CPP := $(BUILDDIR)/scanner.cpp

# Object Files (will be in build directory)
NON_GENERATED_SRCS := $(filter-out $(SRCDIR)/parser.cpp $(SRCDIR)/scanner.cpp, $(SRCS))
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(filter-out $(SRCDIR)/parser.cpp $(SRCDIR)/scanner.cpp, $(SRCS)))
OBJS += $(BUILDDIR)/parser.o $(BUILDDIR)/scanner.o

# Test files
TEST_SAMPLES := $(wildcard $(TESTDIR)/*.txt) 

.PHONY: all clean test

all: directories $(PARSER_CPP) $(PARSER_H) $(LEX_CPP) $(BUILDDIR)/$(TARGET)

directories:
	@mkdir -p $(BUILDDIR)

# Rule to build the final executable
$(BUILDDIR)/$(TARGET): $(OBJS)
	@echo "Linking $@"
	$(CXX) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# Rule for compiling .cpp files into .o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for compiling the generated parser.cpp
$(BUILDDIR)/parser.o: $(PARSER_CPP) $(PARSER_H)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for compiling the generated scanner.cpp
$(BUILDDIR)/scanner.o: $(LEX_CPP)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to generate parser.cpp and parser.h from parser.y (Bison)
$(PARSER_CPP) $(PARSER_H): $(YACC_SRC)
	@echo "Generating parser files from $<"
	bison -dtv -o $(PARSER_CPP) --define=$(PARSER_H) $<
	@mv parser.output $(BUILDDIR)/parser.output || true


# Rule to generate scanner.cpp from scanner.lex (Flex)
$(LEX_CPP): $(LEX_SRC)
	@echo "Generating scanner file from $<"
	flex -o $@ $<

clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILDDIR)

test: $(BUILDDIR)/$(TARGET)
	@echo "Running tests..."
	@for sample in $(TEST_SAMPLES); do \
		echo "Testing $$sample..."; \
		./$(BUILDDIR)/$(TARGET) $$sample; \
		if [ $$? -eq 0 ]; then \
			echo "  $$sample: PASSED"; \
		else \
			echo "  $$sample: FAILED"; \
		fi; \
	done
	@echo "All tests complete."