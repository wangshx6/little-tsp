################################################################################
# NAMES
################################################################################

SRC_DIR = src
OBJSR_DIR = build/release
OBJSD_DIR = build/debug
INCLUDE_DIR = include
EXECUTABLER = littletsp
EXECUTABLED = littletspd

################################################################################
# COMPILER AND COMPILER FLAGS
################################################################################

CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11
CXXFLAGSD = -g
CXXFLAGSR = -O3
CPPFLAGS = -I$(INCLUDE_DIR)

################################################################################
# TARGETS
################################################################################

all: release

release: CXXFLAGS += $(CXXFLAGSR)
release: OBJS_DIR := $(OBJSR_DIR)
release: $(EXECUTABLER)

debug: CXXFLAGS += $(CXXFLAGSD)
debug: OBJS_DIR := $(OBJSD_DIR)
debug: $(EXECUTABLED)

clean:
	rm -rf $(EXECUTABLER) $(EXECUTABLED) *.o

################################################################################
# OBJECTS AND BINARIES
################################################################################

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, %.o, $(wildcard $(SRC_DIR)/*.cpp))

$(EXECUTABLED) $(EXECUTABLER): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJECTS)) -o $@

################################################################################
# INDIVIDUAL RULES
################################################################################

main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/interaction.hpp \
	$(INCLUDE_DIR)/graph.hpp $(INCLUDE_DIR)/common.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJS_DIR)/$@

interaction.o: $(SRC_DIR)/interaction.cpp $(INCLUDE_DIR)/interaction.hpp \
	$(INCLUDE_DIR)/common.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJS_DIR)/$@

basicgraph.o: $(SRC_DIR)/basicgraph.cpp $(INCLUDE_DIR)/basicgraph.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJS_DIR)/$@

graph.o: $(SRC_DIR)/graph.cpp $(INCLUDE_DIR)/graph.hpp \
	$(INCLUDE_DIR)/matrix.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJS_DIR)/$@

fasttsp.o: $(SRC_DIR)/fasttsp.cpp $(INCLUDE_DIR)/graph.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJS_DIR)/$@

opttsp.o: $(SRC_DIR)/opttsp.cpp $(INCLUDE_DIR)/graph.hpp \
	$(INCLUDE_DIR)/pathinfo.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJS_DIR)/$@

naivetsp.o: $(SRC_DIR)/naivetsp.cpp $(INCLUDE_DIR)/graph.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJS_DIR)/$@

pathinfo.o: $(SRC_DIR)/pathinfo.cpp $(INCLUDE_DIR)/pathinfo.hpp \
	$(INCLUDE_DIR)/basicgraph.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJS_DIR)/$@
