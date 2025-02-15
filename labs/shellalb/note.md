

# test commands for tsh
```bash
make tch
./tch -v

# test eval
/bin/head README

# test ctrl+c : add job,  reap child process and delete job
# FOR UBUNTU 22.04/20.04 # FOR UBUNTU 18.04  /usr/bin/tail -fn 5 README
/bin/tail -fn 5 README
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

make test06
# ./sdriver.pl -t trace06.txt -s ./tsh -a "-p"
# #
# # trace06.txt - Forward SIGINT to foreground job.
# #
# tsh> ./myspin 4


make test07
# # trace07.txt - Forward SIGINT only to foreground job.
# tsh> ./myspin 10 &
# # [1] (18634) ./myspin 4 &
# tsh> ./myspin 5
# tsh> jobs
# # [1] (18634) Running ./myspin 4 &


# make test08
# ./sdriver.pl -t trace08.txt -s ./tsh -a "-p"
# #
# # trace08.txt - Forward SIGTSTP only to foreground job.
# #
# tsh> ./myspin 4 &
# [1] (20499) ./myspin 4 &
# tsh> ./myspin 5
# tsh> jobs

# [1] (20499) Running ./myspin 4 &
# [2] (20501) Stopped ./myspin 5 


make test09
# ./sdriver.pl -t trace09.txt -s ./tsh -a "-p"
# #
# # trace09.txt - Process bg builtin command
# #
# tsh> ./myspin 4 &
# [1] (1619) ./myspin 4 &
# tsh> ./myspin 5
# tsh> jobs
# [1] (1619) Running ./myspin 4 &
# [2] (1621) Stopped ./myspin 5 
# tsh> bg %2
# [2] (1621) ./myspin 5 
# tsh> jobs
# [1] (1619) Running ./myspin 4 &
# [2] (1621) Foreground ./myspin 5


make test10
# ./sdriver.pl -t trace10.txt -s ./tsh -a "-p"
# #
# # trace10.txt - Process fg builtin command. 
# #
# tsh> ./myspin 4 &
# [1] (11504) ./myspin 4 &
# tsh> fg %1
# tsh> jobs
# [1] (11504) Stopped ./myspin 4 &
# tsh> fg %1
# tsh> jobs


 make test11
# ./sdriver.pl -t trace11.txt -s ./tsh -a "-p"
# #
# # trace11.txt - Forward SIGINT to every process in foreground process group
# #
# tsh> ./mysplit 4
# tsh> /bin/ps a


make test12
./sdriver.pl -t trace12.txt -s ./tsh -a "-p"
#
# trace12.txt - Forward SIGTSTP to every process in foreground process group
#
# tsh> ./mysplit 4
# tsh> jobs
# [1] (12483) Stopped ./mysplit 4 
# tsh> /bin/ps a


make test13
./sdriver.pl -t trace13.txt -s ./tsh -a "-p"
#
# trace13.txt - Restart every stopped process in process group
#
tsh> ./mysplit 4
tsh> jobs
[1] (12686) Stopped ./mysplit 4 
tsh> /bin/ps a


```

# ps command
```bash
ps uf
```
  To identify if a process is a zombie,look for processes with a Z in the STAT (status) column. and <defunct> in the COMMAND column also indicates a zombie process.

