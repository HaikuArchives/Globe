# This is a simple crutch makefile for the real one in src/
# Globe needs to be installed in order to run properly.
INSTALL_DIR=~/config/non-packaged/bin

# compile the program
default:
	make -C src/

# install files to the locations
install: default
	INSTALL_DIR=$(INSTALL_DIR) make install -C src/
	cp -r src/RHTML/ ~/config/settings/Globe

# uninstall files from locations
uninstall:
	rm -f $(INSTALL_DIR)/Globe
	rm -rf ~/config/settings/Globe
