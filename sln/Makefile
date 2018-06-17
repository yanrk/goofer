# arguments
runlink            = static
platform           = linux/x64



# paths home
project_home       = ..
build_dir          = $(project_home)/sln
bin_dir            = $(project_home)/lib/$(platform)
object_dir         = $(project_home)/.objs
gnu_home           = $(project_home)/gnu



# includes of goofer headers
goofer_inc_path    = $(project_home)/inc
goofer_includes    = -I$(goofer_inc_path)

# includes of cmarkup headers
cmarkup_inc_path   = $(gnu_home)/cmarkup/inc
cmarkup_includes   = -I$(cmarkup_inc_path)

# includes of jsoncpp headers
jsoncpp_inc_path   = $(gnu_home)/jsoncpp/inc
jsoncpp_includes   = -I$(jsoncpp_inc_path)



# all includes that goofer solution needs
includes           = $(goofer_includes)
includes          += $(cmarkup_includes)
includes          += $(jsoncpp_includes)



# source files of goofer solution
goofer_src_path    = $(project_home)/src
goofer_source      = $(filter %.cpp, $(shell find $(goofer_src_path) -depth -name "*.cpp"))

# source files of cmarkup solution
cmarkup_src_path   = $(gnu_home)/cmarkup/src
cmarkup_source     = $(filter %.cpp, $(shell find $(cmarkup_src_path) -depth -name "*.cpp"))

# source files of jsoncpp solution
jsoncpp_src_path   = $(gnu_home)/jsoncpp/src
jsoncpp_source     = $(filter %.cpp, $(shell find $(jsoncpp_src_path) -depth -name "*.cpp"))



# objects of goofer solution
goofer_objects     = $(goofer_source:$(project_home)%.cpp=$(object_dir)%.o)
goofer_objects    += $(cmarkup_source:$(project_home)%.cpp=$(object_dir)%.o)
goofer_objects    += $(jsoncpp_source:$(project_home)%.cpp=$(object_dir)%.o)



# goofer depends libraries
goofer_depends     = -lpthread -ldl



# output libraries
ifeq ($(runlink), static)
	goofer_outputs = $(bin_dir)/libgoofer.a
else ifeq ($(platform), mac)
	goofer_outputs = $(bin_dir)/libgoofer.dylib
else
	goofer_outputs = $(bin_dir)/libgoofer.so
endif



# build output command line
ifeq ($(runlink), static)
	build_command  = ar -rv $(goofer_outputs) $^
else
	build_command  = g++ -std=c++11 -shared -o $(goofer_outputs) $^ $(goofer_depends)
endif



# build targets
targets            = goofer

# let 'build' be default target, build all targets
build    : $(targets)

goofer   : $(goofer_objects)
	@echo
	@echo "@@@@@  start making goofer  @@@@@"
	$(build_command)
	@echo "@@@@@  make goofer success  @@@@@"
	@echo

# build all objects
$(object_dir)/%.o:$(project_home)/%.cpp
	@dir=`dirname $@`;		\
	if [ ! -d $$dir ]; then	\
		mkdir -p $$dir;		\
	fi
	g++ -c -std=c++11 -g -Wall -O1 -pipe -fPIC $(includes) -o $@ $<

clean    :
	rm -rf $(object_dir) $(bin_dir)/libgoofer.*

rebuild  : clean build
