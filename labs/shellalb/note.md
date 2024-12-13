

# test commands for tsh
```bash
make tch
tch -v

# test ctrl+c : add job,  reap child process and delete job
/bin/head README
# FOR UBUNTU 22.04/20.04
/bin/tail -fn 5 README
# FOR UBUNTU 18.04  /usr/bin/tail -fn 5 README
ctrl+c

# test ctrl+z: stop process
/bin/tail -fn 5 README
ctrl+z

make test05
# ./sdriver.pl -t trace05.txt -s ./tsh -a "-p"
# #
# # trace05.txt - Process jobs builtin command.
# #
# tsh> ./myspin 2 &
# [2] (60408)  ./myspin 2 &
# tsh> ./myspin 3 &
# [4] (60436)  ./myspin 3 &
# tsh> jobs
# [1] (60407) Foreground /bin/echo -e tsh> ./myspin 2 \046
# [3] (60409) Foreground /bin/echo -e tsh> ./myspin 3 \046
# [5] (60437) Foreground /bin/echo tsh> jobs

```

# ps command

- ps -aux
  To identify if a process is a zombie,look for processes with a Z in the STAT (status) column. and <defunct> in the COMMAND column also indicates a zombie process.

