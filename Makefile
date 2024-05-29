# Run make to see list of commands.

########################## Executable (PDTE1.out) arguments ##########################
# arg[0]: PDTE1.out
# arg[1]: Party number (0,1,2)
# arg[2]: File containing IP addresses of all parties
# arg[3]: AES key for local randomness generation (each party has a different file)
# arg[4]: AES key for common randomness generation (with next party)

# OPTIONAL ARGUMENTS: If supplied, these arguments take precedence over the ones in src/main.cpp
# 					  Refer to Makefile Parameters section below for details.
# arg[6]: Which predefined network
# arg[7]: What dataset to use
# arg[8]: Which adversarial model


############################## Makefile parameters ######################################
# Location of OPENSSL installation if running over servers without sudo access
# OPEN_SSL_LOC := /data/swagh/conda
# RUN_TYPE {localhost, LAN or WAN} 
RUN_TYPE := localhost
# NETWORK {SecureML, Sarda, MiniONN, LeNet, AlexNet, and VGG16}
NETWORK := AlexNet
# Dataset {MNIST, CIFAR10, and ImageNet}
DATASET	:= CIFAR10
# Security {Semi-honest or Malicious} 
SECURITY:= Semi-honest # Malicious
#########################################################################################




#########################################################################################
CXX=g++
SRC_CPP_FILES     := $(wildcard src/*.cpp src/PDTE/*.cpp )
OBJ_CPP_FILES     := $(wildcard util/*.cpp)
OBJ_FILES    	  := $(patsubst src/%.cpp, src/%.o, $(SRC_CPP_FILES))
OBJ_FILES    	  += $(patsubst util/%.cpp, util/%.o, $(OBJ_CPP_FILES))
HEADER_FILES       = $(wildcard src/*.h src/PDTE/*.h )

# FLAGS := -static -g -O0 -w -std=c++11 -pthread -msse4.1 -maes -msse2 -mpclmul -fpermissive -fpic
FLAGS := -O3 -w -std=c++14 -pthread -msse4.1 -maes -msse2 -mpclmul -fpic
LIBS := -lcrypto -lssl 
OBJ_INCLUDES := -I 'lib_eigen/'  -I 'util/Miracl/' -I 'util/' -I '$(OPEN_SSL_LOC)/include/'
BMR_INCLUDES := -L./ -L$(OPEN_SSL_LOC)/lib/ $(OBJ_INCLUDES) 

out:
	@echo $(SRC_CPP_FILES) 
	@echo $(OBJ_FILES) 

help: ## Run make or make help to see list of options
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

all: PDTE1.out ## Just compile the code
	 

PDTE1.out: $(OBJ_FILES)
	g++ $(FLAGS) -o $@ $(OBJ_FILES) $(BMR_INCLUDES) $(LIBS)

%.o: %.cpp $(HEADER_FILES)
	$(CXX) $(FLAGS) -c $< -o $@ $(OBJ_INCLUDES)

clean: ## Run this to clean all files
	rm -rf PDTE1.out
	rm -rf src/*.o  src/PDTE/*.o  util/*.o

################################# Remote runs ##########################################
terminal: PDTE1.out ## Run this to print the output of (only) Party 0 to terminal
	./PDTE1.out 1 files/IP_$(RUN_TYPE) files/keyB files/keyAB >/dev/null &
	./PDTE1.out 0 files/IP_$(RUN_TYPE) files/keyA files/keyAB 
	@echo "Execution completed"


file: PDTE1.out ## Run this to append the output of (only) Party 0 to file output/3PC.txt
	mkdir -p output
	./PDTE1.out 1 files/IP_$(RUN_TYPE) files/keyB files/keyAB >/dev/null &
	./PDTE1.out 0 files/IP_$(RUN_TYPE) files/keyA files/keyAB >>output/3PC.txt
	@echo "Execution completed"


.PHONY: help

