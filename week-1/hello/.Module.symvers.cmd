cmd_/home/jochem/drvo/Module.symvers := sed 's/ko$$/o/' /home/jochem/drvo/modules.order | scripts/mod/modpost -m -a  -o /home/jochem/drvo/Module.symvers -e -i Module.symvers   -T -
