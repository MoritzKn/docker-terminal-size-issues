FROM alpine

RUN apk add build-base
RUN apk add ncurses

COPY main.c main.c
RUN gcc main.c -o getsize

CMD ["./getsize"]
