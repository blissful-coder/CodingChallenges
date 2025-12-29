CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SRCDIR = src
OBJDIR = obj
TARGET = jp

# Source files (exclude tree_parser.cpp and parse_tree.cpp from main build)
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/lexer.cpp $(SRCDIR)/parser.cpp $(SRCDIR)/token.cpp $(SRCDIR)/file_utils.cpp
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build target
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Test the refactored parser
test: $(TARGET)
	@echo "Testing refactored JSON parser..."
	@echo ""
	@echo "Step 1 tests:"
	@printf "  valid.json: "; ./$(TARGET) tests/step1/valid.json > /dev/null 2>&1 && echo "PASS" || echo "FAIL"
	@printf "  invalid.json: "; ./$(TARGET) tests/step1/invalid.json > /dev/null 2>&1 && echo "FAIL (should be invalid)" || echo "PASS"
	@echo ""
	@echo "Step 2 tests:"
	@printf "  valid.json: "; ./$(TARGET) tests/step2/valid.json > /dev/null 2>&1 && echo "PASS" || echo "FAIL"
	@printf "  valid2.json: "; ./$(TARGET) tests/step2/valid2.json > /dev/null 2>&1 && echo "PASS" || echo "FAIL"
	@printf "  invalid.json: "; ./$(TARGET) tests/step2/invalid.json > /dev/null 2>&1 && echo "FAIL (should be invalid)" || echo "PASS"
	@printf "  invalid2.json: "; ./$(TARGET) tests/step2/invalid2.json > /dev/null 2>&1 && echo "FAIL (should be invalid)" || echo "PASS"
	@echo ""
	@echo "Step 3 tests:"
	@printf "  valid.json: "; ./$(TARGET) tests/step3/valid.json > /dev/null 2>&1 && echo "PASS" || echo "FAIL"
	@printf "  invalid.json: "; ./$(TARGET) tests/step3/invalid.json > /dev/null 2>&1 && echo "FAIL (should be invalid)" || echo "PASS"
	@echo ""
	@echo "Step 4 tests:"
	@printf "  valid.json: "; ./$(TARGET) tests/step4/valid.json > /dev/null 2>&1 && echo "PASS" || echo "FAIL"
	@printf "  valid2.json: "; ./$(TARGET) tests/step4/valid2.json > /dev/null 2>&1 && echo "PASS" || echo "FAIL"
	@printf "  invalid.json: "; ./$(TARGET) tests/step4/invalid.json > /dev/null 2>&1 && echo "FAIL (should be invalid)" || echo "PASS"
	@echo ""
	@echo "All tests completed!"

# Test with verbose output (shows lexer traces)
test-verbose: $(TARGET)
	@echo "Testing refactored JSON parser (verbose)..."
	@echo "Step 1 tests:"
	./$(TARGET) tests/step1/valid.json || true
	./$(TARGET) tests/step1/invalid.json || true
	@echo "\nStep 2 tests:"
	./$(TARGET) tests/step2/valid.json || true
	./$(TARGET) tests/step2/valid2.json || true
	./$(TARGET) tests/step2/invalid.json || true
	./$(TARGET) tests/step2/invalid2.json || true
	@echo "\nStep 3 tests:"
	./$(TARGET) tests/step3/valid.json || true
	./$(TARGET) tests/step3/invalid.json || true
	@echo "\nStep 4 tests:"
	./$(TARGET) tests/step4/valid.json || true
	./$(TARGET) tests/step4/valid2.json || true
	./$(TARGET) tests/step4/invalid.json || true

.PHONY: all clean test
