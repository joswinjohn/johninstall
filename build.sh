cmake -S $HOME/johninstall/ -B $HOME/johninstall/build/ &&
  make -f $HOME/johninstall/build/Makefile -C $HOME/johninstall/build &&
  exec $HOME/johninstall/build/johninstall
