CPP=g++

ifdef DEBUG
	CXXFLAGS=-I. -O0 -g # -march=athlon64 -m32 -msse2 -mfpmath=sse
else
	CXXFLAGS=-I. -O3 # -march=athlon64 -m32 -funroll-all-loops -msse2 -mfpmath=sse
endif

#CXXFLAGS=-I. -O2 -ffast-math -funroll-all-loops

# CXXFLAGS=-I. -O2 -ffast-math -funroll-all-loops -march=athlon-xp -mtune=athlon-xp -msse2 -pipe -mfpmath=sse

# CXXFLAGS=-I. -march=athlon64 -m32 -m3dnow -msse2 -mfpmath=387 -mmmx -O2 -pipe -fforce-addr -fomit-frame-pointer -funroll-loops -frerun-cse-after-loop -frerun-loop-opt -falign-functions=4 -maccumulate-outgoing-args -ffast-math -fprefetch-loop-arrays
DEPS = game_learner.h \
	   game_player.h \
	   game.h \
	   gtp_neural_interface.h \
	   input_layer.h \
	   layer.h \
	   network.h \
	   neuron.h \
	   othello.h \
	   tictactoe.h

# OBJ = game_learner.o game_player.o neural.o gtp_neural_interface.o
OBJ = ggl.o

ifdef HIDDEN_SIZE
	CXXFLAGS += -DHIDDEN_SIZE=$(HIDDEN_SIZE)
endif

ifdef HIDDEN_SIZE_2
	CXXFLAGS += -DHIDDEN_SIZE_2=$(HIDDEN_SIZE_2)
endif

ifdef ETA_VAL
	CXXFLAGS += -DETA_VAL=$(ETA_VAL)
endif

ifdef LAMBDA_VAL
	CXXFLAGS += -DLAMBDA_VAL=$(ETA_VAL)
endif

ifdef ENABLE_VAR_LAMBDA
	CXXFLAGS += -DENABLE_VAR_LAMBDA
endif


%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CXXFLAGS)

all: ggl

ggl: $(OBJ)
	$(CPP) -o $@ $^


clean:
	rm *.o ggl
