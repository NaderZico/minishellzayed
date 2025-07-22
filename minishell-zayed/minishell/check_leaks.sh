valgrind --suppressions="valgrind_readline_leaks_ignore.txt" --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes ./minishell
# -track-fds=yes for knowind fds status errors and leaks if one had opened and not closed 
# --leak-check=full  check memory leaks
# --trace-children=yes attach valgrind to the new chiled has created by fork
# --suppressions="valgrind_readline_leaks_ignore.txt" becouse readline functions has some leaks they are used on history saving and viewing