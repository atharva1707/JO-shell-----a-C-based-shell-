
shell: main.c PrintBase.c Directory.c Process.c redirection.c history.c Input.c builtin.c Pipeline.c header.h
	gcc -o shell main.c PrintBase.c Directory.c Input.c builtin.c Process.c history.c redirection.c Pipeline.c