ALG_SOURCE  := source/algorithm
GUI_SOURCE  := source/gui
CLI_SOURCE  := source/cli
TEST_SOURCE := tests

ALG_BUILD  := build/algorithm
GUI_BUILD  := build/gui
CLI_BUILD  := build/cli
TEST_BUILD := build/tests

GUI_OUTPUT  := bin/gui/Polygonal-Intersections-GUI
CLI_OUTPUT  := bin/cli/Polygonal-Intersections-CLI
TEST_OUTPUT := bin/tests/test

HMTL_COV := htmlcov
GCOVR_COV := gcovr
TRACEFILE := coverage.info

INCLUDES := $(ALG_SOURCE)

ALG_CPPFILES  := $(notdir $(wildcard $(ALG_SOURCE)/*.cpp))
GUI_CPPFILES  := $(notdir $(wildcard $(GUI_SOURCE)/*.cpp))
CLI_CPPFILES  := $(notdir $(wildcard $(CLI_SOURCE)/*.cpp))
TEST_CPPFILES := $(notdir $(wildcard $(TEST_SOURCE)/*.cpp)) $(ALG_CPPFILES)

ALG_OFILES  := $(foreach file,$(ALG_CPPFILES:.cpp=.o),$(ALG_BUILD)/$(file))
GUI_OFILES  := $(foreach file,$(GUI_CPPFILES:.cpp=.o),$(GUI_BUILD)/$(file))
CLI_OFILES  := $(foreach file,$(CLI_CPPFILES:.cpp=.o),$(CLI_BUILD)/$(file))
TEST_OFILES := $(foreach file,$(TEST_CPPFILES:.cpp=.o),$(TEST_BUILD)/$(file))

INCLUDE := $(foreach dir,$(INCLUDES),-I$(dir))

CXX := g++

COMMON_CXXFLAGS := $(INCLUDE) -std=c++23 -Wall -Wextra -Wno-switch
CXXFLAGS := $(COMMON_CXXFLAGS) -O3
TEST_CXXFLAGS := $(COMMON_CXXFLAGS) --coverage

GUI_LDFLAGS := bin/gui/sfml-graphics-2.dll \
               bin/gui/sfml-window-2.dll \
               bin/gui/sfml-system-2.dll \
               -Wl,-subsystem,console

CLI_LDFLAGS := -static-libgcc -static-libstdc++
TEST_LDFLAGS := --coverage

.PHONY: gui cli tests run_tests cov coverage gcovr \
	clean gui_dirs cli_dirs alg_dirs test_dirs

all: gui cli

gui_dirs:
	@mkdir -p $(GUI_BUILD) bin/gui

cli_dirs:
	@mkdir -p $(CLI_BUILD) bin/cli

test_dirs:
	@mkdir -p $(TEST_BUILD) bin/tests

alg_dirs:
	@mkdir -p $(ALG_BUILD)

gui: $(GUI_OUTPUT)
cli: $(CLI_OUTPUT)
tests: $(TEST_OUTPUT)

run_tests: tests
	./bin/tests/test

$(TRACEFILE): run_tests
	lcov --capture --directory $(TEST_BUILD) --output-file $(TRACEFILE)

	lcov --remove $@ '$(shell pwd)/tests/*' \
		'/usr/include/boost/*' '/usr/local/include/boost/*' \
		'/usr/include/*' --output-file $@

coverage: $(TRACEFILE)
	genhtml $(TRACEFILE) --output-directory $(HMTL_COV)

cov: coverage

gcovr: run_tests
	@mkdir -p $(GCOVR_COV)
	gcovr --html-details $(GCOVR_COV)/coverage.html -e tests

clean:
	@echo clean...
	@rm -fr build $(GUI_OUTPUT) $(CLI_OUTPUT) $(TEST_OUTPUT) \
		$(HMTL_COV) $(TRACEFILE) $(GCOVR_COV)

$(GUI_OUTPUT): $(ALG_OFILES) $(GUI_OFILES)
	@echo $(notdir $(GUI_OUTPUT))
	$(CXX) -o $@ $^ $(GUI_LDFLAGS)
	@echo

$(CLI_OUTPUT): $(ALG_OFILES) $(CLI_OFILES)
	@echo $(notdir $(CLI_OUTPUT))
	$(CXX) -o $@ $^ $(CLI_LDFLAGS)
	@echo

$(TEST_OUTPUT): $(TEST_OFILES)
	@echo $(notdir $(TEST_OUTPUT))
	$(CXX) -o $@ $^ $(TEST_LDFLAGS)
	@echo

$(ALG_BUILD)/%.o: $(ALG_SOURCE)/%.cpp | alg_dirs
	@echo $<
	$(CXX) -MMD -MP -MF $(dir $@)$*.d $(CXXFLAGS) -c $< -o $@
	@echo

$(GUI_BUILD)/%.o: $(GUI_SOURCE)/%.cpp | gui_dirs
	@echo $<
	$(CXX) -MMD -MP -MF $(dir $@)$*.d $(CXXFLAGS) -c $< -o $@
	@echo

$(CLI_BUILD)/%.o: $(CLI_SOURCE)/%.cpp | cli_dirs
	@echo $<
	$(CXX) -MMD -MP -MF $(dir $@)$*.d $(CXXFLAGS) -c $< -o $@
	@echo

$(TEST_BUILD)/%.o: $(TEST_SOURCE)/%.cpp | test_dirs
	@echo $<
	$(CXX) -MMD -MP -MF $(dir $@)$*.d $(TEST_CXXFLAGS) -c $< -o $@
	@echo

$(TEST_BUILD)/%.o: $(ALG_SOURCE)/%.cpp | test_dirs
	@echo $<
	$(CXX) -MMD -MP -MF $(dir $@)$*.d $(TEST_CXXFLAGS) -c $< -o $@
	@echo

-include $(ALG_BUILD)/*.d
-include $(GUI_BUILD)/*.d
-include $(CLI_BUILD)/*.d
-include $(TEST_BUILD)/*.d
