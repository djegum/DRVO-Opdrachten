cmd_/home/jochem/drvo/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/jochem/drvo/"$$0) }' > /home/jochem/drvo/hello.mod
