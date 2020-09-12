# s6-overlay-preinit

This is a small program that ensures the needed runtime directory for
[`s6-overlay`](https://github.com/just-containers/s6-overlay) exists and
is owned by the current user.

This should enable `s6-overlay` users to use the `USER` directive in their
dockerfiles. This binary needs the `SETUID` bit enabled so it can
gain privileges to create the `/var/run/s6` directory.

## Usage

`s6-overlay-preinit /var/run`

This will:

* create `/var/run` if needed (ie, using a read-only root filesystem).
* create `/var/run/s6`.
* chown `/var/run/s6` to the current UID/GID (set if using `USER` directive).

## LICENSE

ISC - see `LICENSE.md`
