# Makefile for the CTL Verifier and Visualizer

CC = gcc
CFLAGS = -Wall -Wextra -g

OBJDIR = sim
OUTPUT_DIR = output
# Source files
MODEL_SRC = ctl_model_operations.c
OPERATORS_SRC = ctl_operators.c
VERIFICATION_SRC = ctl_verification.c
MAIN_SRC = ctl_main.c
VIZ_SRC = ctl_visualizer.c

# Object files (now in sim/)
MODEL_OBJ = $(OBJDIR)/$(MODEL_SRC:.c=.o)
OPERATORS_OBJ = $(OBJDIR)/$(OPERATORS_SRC:.c=.o)
VERIFICATION_OBJ = $(OBJDIR)/$(VERIFICATION_SRC:.c=.o)
MAIN_OBJ = $(OBJDIR)/$(MAIN_SRC:.c=.o)
VIZ_OBJ = $(OBJDIR)/$(VIZ_SRC:.c=.o)

# All object files for verifier
VERIFIER_OBJS = $(MODEL_OBJ) $(OPERATORS_OBJ) $(VERIFICATION_OBJ) $(MAIN_OBJ)

# Common header
HEADER = ctl_common.h


all: clean $(OBJDIR) ctl_verifier ctl_visualizer
	./ctl_verifier | tee log
	./improved_script.sh log $(OUTPUT_DIR)
	cp $(OUTPUT_DIR)/markdown/summary.md summary.md
# Ensure sim directory exists before compiling
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)
# Compile each source file to sim/
$(OBJDIR)/%.o: %.c $(HEADER) | $(OBJDIR)
	@echo 
	@echo '*******' Makefile @`date '+%Y-%m-%d-%H:%M:%S'` compiling $< '*******'
	@echo 
	$(CC) $(CFLAGS) -c $< -o $@

# Link verifier
ctl_verifier: $(VERIFIER_OBJS)
	@echo 
	@echo '*******' Makefile @`date '+%Y-%m-%d-%H:%M:%S'` linking $@ '*******'
	@echo 
	$(CC) $(CFLAGS) -o $@ $(VERIFIER_OBJS)

# Link visualizer
ctl_visualizer: $(MODEL_OBJ) $(OPERATORS_OBJ) $(VIZ_OBJ)
	@echo 
	@echo '*******' Makefile @`date '+%Y-%m-%d-%H:%M:%S'` linking $@ '*******'
	@echo 
	$(CC) $(CFLAGS) -o $@ $(MODEL_OBJ) $(OPERATORS_OBJ) $(VIZ_OBJ)

# Clean up
clean:
	@echo 
	@echo '*******' Makefile @`date '+%Y-%m-%d-%H:%M:%S'` cleaning up '*******'
	@echo 
	rm -rf ctl_verifier ctl_visualizer sim/*.o *.dot *.png log summary.md 