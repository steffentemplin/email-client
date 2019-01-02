FROM centos:7

RUN yum -y install dovecot; \
    yum clean all; \
    groupadd -g 1337 vmail; \
    useradd vmail -g 1337 -u 1337 -r -m -d /var/vmail; \
    rm -rf /etc/dovecot/conf.d

RUN /etc/ssl/certs/make-dummy-cert /etc/dovecot/dovecot.pem

ADD dovecot.conf /etc/dovecot/dovecot.conf

CMD ["/usr/sbin/dovecot", "-F"]

EXPOSE 143