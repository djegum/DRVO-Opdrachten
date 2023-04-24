cmd_/home/jochem/drvo/week-1/basic-driver/modules.order := {   echo /home/jochem/drvo/week-1/basic-driver/hello.ko; :; } | awk '!x[$$0]++' - > /home/jochem/drvo/week-1/basic-driver/modules.order
