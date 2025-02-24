FROM archlinux

RUN pacman --noconfirm -Syu \
  && pacman --noconfirm -S fmt ncurses nlohmann-json

COPY build/johninstall /tmp
ENV TERM xterm
RUN ./tmp/johninstall
