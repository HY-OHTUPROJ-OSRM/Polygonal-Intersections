SOURCES       := source
INCLUDES      := $(SOURCES)
BUILD         := build
TARGET        := Polygonal-Intersections

export OUTPUT := $(CURDIR)/bin/$(TARGET)
export VPATH  := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

CFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

export OFILES := $(foreach file,$(CPPFILES:.cpp=.o) $(CFILES:.c=.o),$(BUILD)/$(file))
export INCLUDE := $(foreach dir,$(INCLUDES),-I$(dir))

CXX := C:/winlibs-x86_64-posix-seh-gcc-10.1.0-llvm-10.0.0-mingw-w64-7.0.0-r3/mingw64/bin/g++.exe

CXXFLAGS := $(INCLUDE) -std=c++20 -O3 -Wall -Wextra -Wno-switch

LDFLAGS := bin/sfml-graphics-2.dll \
           bin/sfml-window-2.dll \
           bin/sfml-system-2.dll \
           -Wl,-subsystem,console

$(OUTPUT): $(OFILES)
	@echo linking...
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD)/%.o: %.cpp
	@echo $(notdir $<)
	$(CXX) -MMD -MP -MF $(BUILD)/$*.d $(CXXFLAGS) -c $< -o $@

-include $(BUILD)/*.d