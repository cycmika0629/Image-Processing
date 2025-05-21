# ompiler & Linker settings
CXX = g++
CXXFLAGS = -I ./inc -I ./inc/filters -I ./bit_field_filter -I ./third-party/CImg -I/opt/homebrew/include -I ./Data-Loader -std=c++11 -I/opt/X11/include -I/opt/homebrew/opt/jpeg/include 

OPTFLAGS = -march=native -flto -funroll-loops -finline-functions -ffast-math -O3
WARNINGS = -g -Wall
LINKER = -lm -lpthread -lX11 -ljpeg -lpng \
         -L/usr/X11R6/lib \
         -L/opt/X11/lib \
         -L/opt/homebrew/opt/jpeg/lib \
         -L./third-party/libjpeg


# Valgrind for memory issue
CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all --track-origins=yes 

# Source files and object files
SRCDIR = src
OBJDIR = obj
INCDIR = inc
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/filters/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.d,$(SRCS))

# Control the build verbosity
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

.PHONY: all install check clean

# Name of the executable
TARGET = Image_Processing Data_Loader_Example

all: $(TARGET)

$(OBJDIR):
	@mkdir $(OBJDIR)
$(OBJDIR)/filters:
	@mkdir -p $(OBJDIR)/filters


Image_Processing: main.cpp $(OBJS) $(OBJDIR)/data_loader.o
	$(VECHO) "	LD\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LINKER)

Data_Loader_Example: data_loader_demo.cpp $(OBJDIR)/data_loader.o
	$(VECHO) "	LD\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LINKER)

# Include generated dependency files
-include $(DEPS)

# Compilation rule for object files with automatic dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(OBJDIR)/filters Makefile
	$(VECHO) "	CC\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) -MMD -c $< -o $@

# Compilation rule for data_loader.o with explicit dependencies
$(OBJDIR)/data_loader.o: ./Data-Loader/data_loader.cpp ./Data-Loader/data_loader.h | $(OBJDIR) Makefile
	$(VECHO) "	CC\t$@\n"
	$(Q)$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) -MMD -c $< -o $@



install:
	$(VECHO) "Installing third party dependencies\n"
	$(Q)chmod +x scripts/clone_env.sh  
	$(Q)./scripts/clone_env.sh  > /dev/null 2>&1
	$(VECHO) "Finished installing third party dependencies!!\n"

check:
	$(CHECKCC) $(CHECKFLAGS) ./Image_Processing

clean:
	rm -rf $(OBJDIR) $(TARGET)
