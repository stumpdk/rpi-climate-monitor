FROM hypriot/rpi-mysql

VOLUME /conf:/etc/mysql/conf.d
ADD init_db.sh /tmp/init_db.sh
ADD init.sql /tmp/init.sql
RUN chmod +x /tmp/init_db.sh && /tmp/init_db.sh