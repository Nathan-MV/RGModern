# -----------------------------------------------
# Packages settings
# -----------------------------------------------
include ext/settings.txt
GITHUB_URL ?= https://github.com

path_packages := ./packages
path_download := $(path_packages)/download

# -----------------------------------------------
# Ruby static library
# -----------------------------------------------
RUBY_VERSION := $(RUBY_MAJOR_VERSION).$(RUBY_MINOR_VERSION).$(RUBY_PATCH_VERSION)
RUBY_LIBRARY_VERSION := $(RUBY_M_VERSION)0

url_ruby_static := https://cache.ruby-lang.org/pub/ruby/$(RUBY_MAJOR_VERSION).$(RUBY_MINOR_VERSION)/ruby-$(RUBY_VERSION).tar.gz
zip_ruby_static := $(path_download)/ruby-$(RUBY_VERSION).tar.gz
path_ruby_static := $(path_packages)/ruby-$(RUBY_VERSION)

lib_ruby_static := $(path_ruby_static)/libruby$(RUBY_LIBRARY_VERSION).a

lib_ruby := ./third_party/ruby$(RUBY_M_VERSION)
configure_ruby = --disable-install-doc --disable-jit-support --disable-rubygems --with-static-linked-ext --with-out-ext=openssl --without-gmp

# -----------------------------------------------
# Fedora packages (including SDL2 library)
# -----------------------------------------------
pkgs = SDL2-dev SDL2_ttf-dev SDL2_image-dev SDL2_mixer-dev SDL2_gfx-dev SDL2_net-dev zlib libzip libyaml
pkgs_fedora = $(addprefix $(FEDORA_PACKAGE_PREFIX),$(pkgs))
pkgs_fedora += bison libffi gmp upx p7zip vim unzip

# -----------------------------------------------
# Misc
# -----------------------------------------------
miscs = history resource SDL2*.dll
miscs += third_party/ruby$(RUBY_M_VERSION) Doxyfile
miscs += Project1/Audio Project1/Graphics Project1/Graphics.zip
miscs += Project1/*.dll Project1/RPGXP.chm Project1/RPGXP.chw 

# -----------------------------------------------
# Tasks
# -----------------------------------------------
.PHONY: all envs ruby doc

envs:
	apt install -y $(pkgs_fedora)
	mkdir -p $(path_download)

ruby : $(lib_ruby)

$(zip_ruby_static) :
	mkdir -p $(path_download)
	wget -O $(zip_ruby_static) $(url_ruby_static)
	touch $(zip_ruby_static)

$(lib_ruby_static) : $(zip_ruby_static)
	rm -rf $(path_ruby_static)
	tar -C $(path_packages)/ -xzf $(zip_ruby_static)
	cd $(path_ruby_static)/ && ./configure $(configure_ruby)
	cd $(path_ruby_static)/ && make -j4
	touch $(lib_ruby_static)

$(lib_ruby) : $(lib_ruby_static)
	mkdir -p $(lib_ruby) $(lib_ruby)/include $(lib_ruby)/lib $(lib_ruby)/script
	cp -r $(path_ruby_static)/include/ruby* $(lib_ruby)/include
	cp -r $(path_ruby_static)/.ext/include/ruby/config.h $(lib_ruby)/include/ruby/config.h
	cp -r $(path_ruby_static)/*.a $(path_ruby_static)/ext/*/*.a $(lib_ruby)/lib
	cp -r $(path_ruby_static)/ext/fiddle/lib/* $(lib_ruby)/script
	sed -i -r "s/^require '.+'/\# \0/g" $(lib_ruby)/script/*.rb
	sed -i -r "s/^require '.+'/\# \0/g" $(lib_ruby)/script/*/*.rb
	cp -r $(lib_ruby)/script/* ./src/script

misc.7z :
	7z a $@ $(miscs)

Graphics.zip :
	cd Project1 && 7z a -tzip ../Graphics.zip Graphics -scsUTF-8 -p123

doc : manual.md
	# curl https://api.csswg.org/bikeshed/ -F file=@manual.md -F force=1 > manual.html
	ruby ./ext/rgm_defines_gen.rb > ./src/script/rgm_defines.rb
