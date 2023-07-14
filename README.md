# fwcwrap

This is a simple wrapper for a command that needs to executed as `root`,
but called from a PHP web page.

It is installed with set-user-id permission, as root, so can be called
by any user. This example constructs a `firewall-cmd` command to add or remove
a firewall rule, but the technique could be adapted to provide any selection
of specific privileged commands.

To compile:

```
$ make
```

To install, as root:

```
# make install
```

The example PHP page `ipaddr.php` shows how it can be used. This page gets the caller's
IP address and checks whether it is in the `remote` zone, displaying the result.
The caller can then click on a button to add or remove their current address by default.

**Note that this is just an example, and such a page should only be accessible
in an authenticated way.**
