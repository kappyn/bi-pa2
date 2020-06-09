.PHONY            = all compile run memcheck fwatch clean dev linecnt
.DEFAULT_GOAL     = dev

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

# colors
COLOR_DEF = \033[0m
COLOR_MSG = \033[1;36m

# memory management settings
CHECK      		  = valgrind
CHECKFLAGS 		  = --leak-check=full --tool=memcheck

# tasks
all: clean dep compile doc
	@echo "$(COLOR_MSG)Makefile routine completed.$(COLOR_DEF)"

dev: clean dep compile fwatch
	@echo "$(COLOR_MSG)Development makefile routine completed.$(COLOR_DEF)"

compile: $(TARGET)
	@echo "$(COLOR_MSG)Source files have been compiled.$(COLOR_DEF)"

run: $(TARGET)
	@echo "$(COLOR_MSG)Starting executable..$(COLOR_DEF)"
	./$(TARGET)

memcheck: $(TARGET)
	@echo "$(COLOR_MSG)Starting memory check..$(COLOR_DEF)"
	$(CHECK) $(CHECKFLAGS) ./$(TARGET)

fwatch:
	@echo "$(COLOR_MSG)Starting dev filewatcher..$(COLOR_DEF)"
	./filewatch

linecnt:
	@echo "$(COLOR_MSG)Counting lines..$(COLOR_DEF)"
	find "./src/" -type f -name "*.cpp" -o -name "*.hpp" | xargs wc -l

clean:
	rm -rf $(TARGET) $(BUILD_DIR)/ $(DOC_DIR)/ src/*.o  src/*/*.o 2>/dev/null
	@echo "$(COLOR_MSG)Previous builds and object files have been cleaned.$(COLOR_DEF)"

dep:
	find src/ -name *.cpp | xargs $(CXX) -MM > Makefile.d
	sed -i -E 's/^.*.o:/$(BUILD_DIR)\/&/g' Makefile.d
	@echo "$(COLOR_MSG)Source dependencies have been updated.$(COLOR_DEF)"

doc: $(SOURCE_DIR)/main.cpp $(wildcard **/*.hpp)
	rm -rf $(DOC_DIR)
	$(MKDIR) $(DOC_DIR)
	doxygen Doxyfile
	@echo "$(COLOR_MSG)Documentation has been created.$(COLOR_DEF)"
	google-chrome doc/index.html

$(TARGET): $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $^ -o $@
	@echo "$(COLOR_MSG)Object files linked successfully.$(COLOR_DEF)"

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/**/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -c -o $@

-include Makefile.d