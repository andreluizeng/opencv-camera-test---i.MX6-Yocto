#before buildint it don't forget to export ROOTF_DIR and TOOLCHAIN (only if they are uncommented here)

APPNAME			= camera_test
DESTDIR			= ../bin
SRCDIR			= ../src

CXX			= $(CROSS_COMPILE)g++
DEL_FILE		= rm -rf
CP_FILE			= cp -rf    

ROOTFS_DIR = $(SDKTARGETSYSROOT)

TARGET_PATH_LIB 	= $(ROOTFS_DIR)/usr/lib
TARGET_PATH_INCLUDE 	= $(ROOTFS_DIR)/usr/include



# x11 flags
ifeq ($(EGL_FLAVOR),x11)
CFLAGS			= -DLINUX -DUSE_SOC_MX6 -Wall -O2 -fsigned-char -march=armv7-a -mfpu=neon -mfloat-abi=hard \
			  -DEGL_USE_X11 -DGPU_TYPE_VIV -DGL_GLEXT_PROTOTYPES -DENABLE_GPU_RENDER_20 \
			  -I. -I$(TARGET_PATH_INCLUDE) \
                          -I$(TARGET_PATH_INCLUDE)/glib-2.0 -I$(TARGET_PATH_LIB)/glib-2.0/include \
                          -I$(TARGET_PATH_INCLUDE)/libxml2 \

LFLAGS			= -Wl,--library-path=$(TARGET_PATH_LIB),-rpath-link=$(TARGET_PATH_LIB) -lm \
			   -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml
 

# fb flags
else
CFLAGS			= -DLINUX -DUSE_SOC_MX6 -Wall -O2 -fsigned-char -march=armv7-a -mfpu=neon -mfloat-abi=hard \
			  -DEGL_API_FB -DGPU_TYPE_VIV -DGL_GLEXT_PROTOTYPES -DENABLE_GPU_RENDER_20 \
			  -I. -I$(TARGET_PATH_INCLUDE) \
                          -I$(TARGET_PATH_INCLUDE)/glib-2.0 -I$(TARGET_PATH_LIB)/glib-2.0/include \
                          -I$(TARGET_PATH_INCLUDE)/libxml2 \

LFLAGS			= -Wl,--library-path=$(TARGET_PATH_LIB),-rpath-link=$(TARGET_PATH_LIB) -lm \
			   -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml

endif


OBJECTS			= $(APPNAME).o 

first: all

all: $(APPNAME)

$(APPNAME): $(OBJECTS) 
	$(CC) $(LFLAGS) -o $(DESTDIR)/$(APPNAME) $(OBJECTS)  

$(APPNAME).o: $(APPNAME).c
	$(CC) $(CFLAGS) -c -o $(APPNAME).o $(APPNAME).c


clean:
	$(DEL_FILE) $(SRCDIR)/$(OBJECTS)
	$(DEL_FILE) $(DESTDIR)/$(OBJECTS)
	$(DEL_FILE) $(DESTDIR)/*~ $(DESTDIR)/*.core
	$(DEL_FILE) $(DESTDIR)/$(APPNAME)
	
distclean: clean
	$(DEL_FILE) $(DESTDIR)/$(APPNAME)

install: all
	#${CP_FILE} $(SRCDIR)/$(APPNAME) $(DESTDIR)/$(APPNAME)
	#${CP_FILE} $(DESTDIR)/$(APPNAME) $(ROOTFS_DIR)/home/opencv

