cmake .. -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`;
make;
kdesu make install;
kbuildsycoca4;
kquitapp plasma-desktop;
sleep 1s;
plasma-desktop;
