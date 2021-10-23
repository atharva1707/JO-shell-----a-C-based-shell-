## C-shell

a C-based shell with features like pipelining, input-output redirection, process creation, job management, signal handling, and few special in-built commands

### Running the shell
Run the command `make` and `./shell` to run the program 
>  - Multiple commands can be run seprated by a semicolon '`;`'
>  - Pipelining can be done using '`|`'
>  - input-output redirection using '`<`', '`>`' , '`>>`'

<br>

### In-built commands
>  - You can use the `jobs` command to keep a track of all the background processes along with the job number
>  - you can use the `fg` command to bring a background command to foreground
>  - you can use the `bg` command to change the state of a stopped  background process to running
>  - the `sig` command can be used to send a particular signal to a speific process
>  - the `pinfo` command can be used to gather information about a process 
>  - commands like `echo` , `ls` (with flags) , `pwd` , `cd`  are also implemented manually

<br>

### Syntax
```
    pinfo <process id>
    jobs
    fg job_number
    bg job_number
    sig job_number signal_number
```