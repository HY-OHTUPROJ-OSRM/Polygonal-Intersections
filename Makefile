ALG_SOURCE := source/algorithm
GUI_SOURCE := source/gui
CLI_SOURCE := source/cli

ALG_BUILD := build/algorithm
GUI_BUILD := build/gui
CLI_BUILD := build/cli

GUI_OUTPUT := bin/gui/Polygonal-Intersections-GUI
CLI_OUTPUT := bin/cli/Polygonal-Intersections-CLI

INCLUDES := $(ALG_SOURCE)

ALG_CPPFILES := $(notdir $(wildcard $(ALG_SOURCE)/*.cpp))
GUI_CPPFILES := $(notdir $(wildcard $(GUI_SOURCE)/*.cpp))
CLI_CPPFILES := $(notdir $(wildcard $(CLI_SOURCE)/*.cpp))

ALG_OFILES := $(foreach file,$(ALG_CPPFILES:.cpp=.o),$(ALG_BUILD)/$(file))
GUI_OFILES := $(foreach file,$(GUI_CPPFILES:.cpp=.o),$(GUI_BUILD)/$(file))
CLI_OFILES := $(foreach file,$(CLI_CPPFILES:.cpp=.o),$(CLI_BUILD)/$(file))

INCLUDE := $(foreach dir,$(INCLUDES),-I$(dir))

ifeq ($(shell test -e g++ && echo -n exists),exists)
	CXX := g++
else
	CXX := C:/winlibs-x86_64-posix-seh-gcc-10.1.0-llvm-10.0.0-mingw-w64-7.0.0-r3/mingw64/bin/g++.exe
endif

CXXFLAGS := $(INCLUDE) -std=c++20 -O3 -Wall -Wextra -Wno-switch

GUI_LDFLAGS := bin/gui/sfml-graphics-2.dll \
               bin/gui/sfml-window-2.dll \
               bin/gui/sfml-system-2.dll \
               -Wl,-subsystem,console

CLI_LDFLAGS := -static-libgcc -static-libstdc++

.PHONY: gui cli gui_dirs cli_dirs alg_dirs clean

all: gui cli

gui_dirs:
	@mkdir -p $(GUI_BUILD) bin/gui

cli_dirs:
	@mkdir -p $(CLI_BUILD) bin/cli

alg_dirs:
	@mkdir -p $(ALG_BUILD)

gui: $(GUI_OUTPUT)
cli: $(CLI_OUTPUT)

clean:
	@echo clean...
	@rm -fr build $(GUI_OUTPUT) $(CLI_OUTPUT)

$(GUI_OUTPUT): $(ALG_OFILES) $(GUI_OFILES)
	@echo $(notdir $(GUI_OUTPUT))
	$(CXX) -o $@ $^ $(GUI_LDFLAGS)
	@echo

$(CLI_OUTPUT): $(ALG_OFILES) $(CLI_OFILES)
	@echo $(notdir $(CLI_OUTPUT))
	$(CXX) -o $@ $^ $(CLI_LDFLAGS)
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

-include $(ALG_BUILD)/*.d
-include $(GUI_BUILD)/*.d
-include $(CLI_BUILD)/*.d
