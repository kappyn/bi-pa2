# sources
TARGET     		  = kroupkev
BUILD_DIR  		  = build
SOURCE_DIR 		  = src
DOC_DIR			  = doc

SOURCES 		  = $(wildcard $(SOURCE_DIR)/*.cpp $(SOURCE_DIR)/*/*.cpp)
HEADERS 		  = $(wildcard $(SOURCE_DIR)/*.hpp $(SOURCE_DIR)/*/*.hpp)
OB_TMP    	      = $(notdir $(SOURCES:$(SOURCE_DIR)/%.cpp=%.o))
OBJECTS    	      = $(OB_TMP:%.o=$(BUILD_DIR)/%.o)

# compiler settings
CXX        		  = g++
CXX_FLAGS  		  = -std=c++14 -Werror -Wall -pedantic -Wno-long-long
MKDIR      		  = mkdir -p

# memory management settings
CHECK      		  = valgrind
CHECKFLAGS 		  = --leak-check=full --tool=memcheck

# tasks
.PHONY: all
all: compile run
	@echo "Finished.."

.PHONY: compile
compile: $(TARGET)
	@echo "Compiled.."

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: memcheck
memcheck: $(TARGET)
	$(CHECK) $(CHECKFLAGS) ./$(TARGET)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(BUILD_DIR)/ $(DOC_DIR)/ src/*.o  src/*/*.o tmp.txt 2>/dev/null

dep:
	find src/ -name *.cpp | xargs $(CXX) -MM > Makefile.d
	@echo "Dependencies updated."

doc: $(SOURCE_DIR)/main.cpp $(wildcard **/*.hpp)
	rm -rf $(DOC_DIR)
	$(MKDIR) $(DOC_DIR)
	doxygen Doxyfile
	@echo "Documentation updated."
	google-chrome doc/index.html

$(TARGET): $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/**/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -c -o $@

#-include Makefile.d