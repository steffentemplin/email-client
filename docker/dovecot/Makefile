NAME = local/dovecot
VERSION = 0.1

.PHONY: build build-nocache run push login kill

build:
	docker build -t $(NAME):$(VERSION) --rm image

build-nocache:
	docker build -t $(NAME):$(VERSION) --no-cache --rm image

push: 
	docker push $(NAME):$(VERSION)

run:
	docker run --name=dovecot -d -p 143:143 $(NAME):$(VERSION)

login:
	docker exec -it dovecot bash

kill:
	docker kill dovecot || true
	docker rm dovecot
