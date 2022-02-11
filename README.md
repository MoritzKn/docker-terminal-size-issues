# Terminal Size Issues in Docker

## Problem

`ioctl(STDIN_FILENO, TIOCGWINSZ, &ws)` returns a size of 0 columns and 0 lines and no error code when run in docker.

When using `tput` you see 24 lines and 80 columns. I think the underlying issue is the same but `tput` seems to fall back to a default value.

## Reproduction

Using `ioctl`:

```sh
gcc main.c -o getsize && ./getsize
# --> Should return correct terminal size

docker build . -t getsize && docker run -t getsize ./getsize
# --> Should return 0 for lines and size and result
```

Using `tput`:

```sh
tput lines
# --> Should return the correct number of terminal lines

docker build . -t getsize && docker run -t getsize tput lines
# --> Should return 24
```

## The Underlying Issue

> `docker run -it` (`docker run`) is basically a shorthand implemented in the client that consists of three separate actions;
>
> - docker create
> - docker start
> - (if -i / --interactive): docker attach

[@haJeztah on GitHub](https://github.com/moby/moby/issues/25450#issuecomment-238216897)

This means it takes a moment for the terminal to be resized to the correct resolution.

See also:
- [Docker number of lines in terminal changing inside docker
 on StackOverflow](https://stackoverflow.com/questions/38786615/docker-number-of-lines-in-terminal-changing-inside-docker/38825323#38825323)

It seems like this was fixed by [moby/moby#37172](https://github.com/moby/moby/pull/37172) in [Docker 18.06.0-ce](https://github.com/docker/docker-ce/releases/tag/v18.06.0-ce) but I'm still seeing this in `Docker 20.10.8`.

Hance I opend a [new issue](https://github.com/moby/moby/issues/43229).

## Workaround

Add a short sleep to the run command:

```sh
docker build . -t getsize && docker run -t getsize sh -c 'sleep 0.01 && ./getsize'
```

or in the Dockerfile:

```Dockefile
CMD ["sh", "-c", "sleep 0.01 && ./getsize"]
```

This also works with `tput`:
```sh
docker build . -t getsize && docker run -t getsize sh -c 'sleep 0.01 && tput lines'
```
