
default:	build

clean:
	rm -rf Makefile objs

.PHONY:	default clean

build:
	$(MAKE) -f objs/Makefile

install:
	$(MAKE) -f objs/Makefile install

modules:
	$(MAKE) -f objs/Makefile modules

upgrade:
	/tmp/desert/nginx/sbin/nginx -t

	kill -USR2 `cat /tmp/desert/nginx/logs/nginx.pid`
	sleep 1
	test -f /tmp/desert/nginx/logs/nginx.pid.oldbin

	kill -QUIT `cat /tmp/desert/nginx/logs/nginx.pid.oldbin`

.PHONY:	build install modules upgrade
