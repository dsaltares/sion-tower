# Nombre del proyecto
PROYECTO := siontower
# Directorios
OBJDIR := obj
SRCDIR := src
HDRDIR := include

# MYGUI PATHS
MYGUIENGINE := /home/david/programacion/MyGUI_3.2.0_RC1/MyGUIEngine/include
MYGUIPLATFORM := /home/david/programacion/MyGUI_3.2.0_RC1/Platforms/Ogre/OgrePlatform/include
MYGUILIB := /home/david/programacion/MyGUI_3.2.0_RC1/lib

#Compilador y flags
CXX := g++
CXXFLAGS := -Wall `pkg-config --cflags OGRE` -fexceptions -I$(HDRDIR)

#Fichero de dependencias
DEPFILE := dependencias

# Flags del enlazador
LDFLAGS := `pkg-config --libs OGRE` -L/bin/Debug -L/bin/Release 
LDFLAGS += -lGL  -lstdc++ -lOgreMain -lOIS
LDFLAGS += -lSDL -lSDL_mixer
LDFLAGS += -lMyGUI.OgrePlatform -lMyGUIEngine -lfreetype

LIBS = -lMyGUI.OgrePlatform -lMyGUIEngine -lfreetype

# Colores para la salida de consola
COLOR_FIN := \033[00m
COLOR_OK := \033[01;32m
COLOR_ERROR := \033[01;31m
COLOR_AVISO := \033[01;33m
COLOR_COMP := \033[01;34m
COLOR_ENL := \033[01;35m

# Modo de compilación, por defecto debug
ifeq ($(modo), release)
   CXXFLAGS += -O2 -D_RELEASE
   LDFLAGS += -s
else
   modo := debug
   CXXFLAGS += -g -D_DEBUG
   LDFLAGS += -g 
endif

# Seleccionamos ficheros fuente
SRCS := $(notdir $(shell ls -t $(SRCDIR)/*.cpp))
OBJS := $(addprefix $(OBJDIR)/, $(addsuffix .o,$(basename $(SRCS))))

.PHONY:all
all: informacion gen_deps $(PROYECTO)

# Comprobamos el modo de compilación
informacion:
ifneq ($(modo),release)
ifneq ($(modo),debug)
	@echo ''
	@echo -e '$(COLOR_ERROR)Modo de compilación inválido.$(COLOR_FIN)'
	@echo "Por favor usa 'make [modo=release|debug]'"
	@echo ''
	@exit 1
endif
endif
	@echo ''
	@echo -e 'Compilando en modo $(COLOR_AVISO)"$(modo)"$(COLOR_FIN)'
	@echo '..........................'
	@echo ''

gen_deps:
	@echo ''
	@echo -e '$(COLOR_AVISO)Generando dependencias$(COLOR_FIN)...'
	@$(CXX) -MM -I$(HDRDIR) $(shell ls -t $(SRCDIR)/*.cpp) | sed 's/^\([a-zA-Z]\+.o:\)/$(OBJDIR)\/\1/g' > $(DEPFILE)
	@echo ''

# Compilación
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo -e '$(COLOR_COMP)Compilando$(COLOR_FIN)... $(notdir $<)'
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	
# Enlazado
$(PROYECTO): $(OBJS)
	@echo ''
	@echo -e '$(COLOR_ENL)Enlazando$(COLOR_FIN)...'
	@echo ''
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS) 
	@echo -e '$(COLOR_OK)Terminado.$(COLOR_FIN)'
	@echo ''

# Limpiado del directorio
.PHONY:clean
clean:
	@echo ''
	@echo -e '$(COLOR_AVISO)Limpiando$(COLOR_FIN)...'
	@echo ''
	rm $(DEPFILE) $(OBJS) $(PROYECTO) $(OBJDIR) *~ -rf
	@echo ''
	@echo -e '$(COLOR_OK)Terminado.$(COLOR_FIN)'
	@echo ''

-include $(DEPFILE)
