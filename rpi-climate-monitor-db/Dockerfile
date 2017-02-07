FROM hypriot/rpi-mysql

VOLUME /conf:/etc/mysql/conf.d
ADD init_db.sh /tmp/init_db.sh
RUN chmod +x /tmp/init_db.sh && /tmp/init_db.sh