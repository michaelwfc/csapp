

# test commands for tsh
```bash
make tch
tch -v

# test ctrl+c : add job,  reap child process and delete job
/bin/tail -fn 5 README 
ctrl+c

# test ctrl+z: stop process
/bin/tail -fn 5 README
ctrl+z


```

# ps command

- ps -aux
  To identify if a process is a zombie,look for processes with a Z in the STAT (status) column. and <defunct> in the COMMAND column also indicates a zombie process.

