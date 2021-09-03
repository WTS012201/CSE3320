
Sample and Example Code:

The myshell code is written in "standard" C, and has been tested on several campus computers, various Linux,
and Raspberry Pi.
IT IS AN EXAMPLE. It is not well written (deliberately) but it is functional.
It has many problems, some that we will discuss later, some here.
It uses the C "system()" function which creates a new shell and passes arguments to that new shell.
This call is usually a security vulnerability, and poor practice, and very soon we will discuss how to improve it,
but, for now, it works OK. (don't worry, it isn't a security problem for your Raspberry Pi, but would be for your server.)
There are fixed length buffers, another vulnerability, and poor programming practice.
Input is not handled very well (output is also not very good looking.)
There are system calls to the OS kernel (such as "chdir" and "opendir" and others) that don't check for possible errors.

These problems are intentional. Some you should fix in your assignment (checking for error conditions, for example).

Some you can improve using a "portable" I/O terminal interface (such as "Curses").
A small example program using Curses is also included. You should not mix Curses I/O and standard C 
(it probably won't work, but you can try it.)
If you use Curses, you will need to get the curses library (or object) and make sure you link it with your
executable (something like: gcc <something> -lncurses)
A place to look:
http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/

