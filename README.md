# Shell

## Brief

Custom **shell** which has `cd`, `echo`, `pwd`, `ls`, `pinfo`, `remindme`, `clock`, `setenv`, `unsetenv`, `jobs`.
Piping, redirection, background processes also implemented.

## How to use commands

- `cd` : cd _directory\_name_
- `echo` : echo _string_
- `pwd` : pwd
- `ls` : ls {-l, -a} _directory\_name_
- `pinfo` : pinfo <process_id>
- `remindme` : remindme _time\_in\_seconds_ _string_
- `clock` : clock
- `setenv` : setenv _variable\_name_ _value_
- `unsetenv` : unsetenv _variable\_name_
- `jobs` : jobs
- piping : as done normallY
- redirection : as done normally
- background : as done normally


## To start the shell

- Compile:
	- `make`
- Run:
	- `./main`
- Remove:
    - `make clean`

## Commands and their Files

- **cd**: `cd.c`
- **echo**: `main.c`
- **pwd**: `main.c`
- **ls**: `ls.c`
- **pinfo**: `pinfo.c`
- **remindme**: `remindme.c`
- **clock**: `clock.c`
- **background (&)**: `system_commands.c`
- **setenv**:`setenv.c`
