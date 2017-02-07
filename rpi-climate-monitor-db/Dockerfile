FROM hypriot/rpi-mysql

VOLUME /conf:/etc/mysql/conf.d
ADD init_db.sh /tmp/init_db.sh
RUN /tmp/init_db.sh