# cs327hw04
-------------------------------------------------------------

Ian Malerich - Tools for Digital Sound.
Homework 4 for Computer Science 327 Fall 2015.

# Project Structure
-------------------------------------------------------------

imaudio/
    Shared code between all projects.

sndinfo/

    Sound info generation project.

sndcat/

    Sound contatination project.

sndmix/

    Sound mixture project.

sndgen/

    Sound generator project.

bin/

    Generated binaries.
	Not included in the zip generated by
	the 'zip' and 'zip-nodoc' targets of
	the root Makefile.

lib/
	Staticly linked library for shared code.
	A static library was chosen to keep each
	program independent of all others.

docs/
	Documentation generated the by 'docs' target
	of the root make file. This directory will automatically
	be created by the 'zip' target as well, and should be
	available for use during grading.

# Makefiles
-------------------------------------------------------------

Each project contains its own Makefile and can
be compiled independently of all other binaries.
A makefile is included in the root directory to compile
all projects and generate binaries, as well
as various other utility targets. 
Generated binaries can be found in the bin/ directory.

# A Note to the Grader
-------------------------------------------------------------
This project makes use of a few C++11 features in order to
aid in memory management. STL containers are used frequently
to automate the memory management of any large collection,
a noteable example of their use can be found in the
AudioFile and Channel classes. Move constructors were also
implemented for all major class that store large ammounts of
data, if unfamiliar these allow classes to be efficiently moved
between references without having to resort to pointers to 
move the larger data (in essence, pointers are copied to new
objects, then set to NULL from the source reference).
These ideas combined with the use of references means this
project uses very few points, however it should behave very
efficiently with its memory usage.

The zip target of the root makefile will automatically generate
Doxygen style documentation files. This means the archive 
submitted will have a 'docs' directory when extracted.
To browse the documentation for all the classes simply open
the ./docs/html/index.html in your browser. I hope this 
aids in a quick grading process.

# Install
-------------------------------------------------------------
Binaries can be installed to the user's /usr/bin/ directory
with the 'install' target of the root Makefile
(will ask for sudo). These may be removed
with the 'uinstall' target.

# Documentation
-------------------------------------------------------------

With doxxygen installed on your system. Documentation 
for this project can be generated by calling the 'docs'
target of the root Makefile.

Documentation will also be generated by the 'zip' target
used to generate the archive file for submission.
Documentation may be viewed by opening docs/html/index.html
in your browser of choice.
