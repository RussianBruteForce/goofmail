# goofmail

## How-to

Create files (args in \[brackets\] may be skipped):

 * `persons.csv` — list of recepients: `email,name` (one on each line)
 * `smtp.csv` — list of smtp servers: `server,username,password,[from]` (one on each line)
 * `proxies.csv` — list of proxies: `server:port,[username],[password]` (one on each line)
 * `headers` — message headers, `Sample header, hi <name>` (one on each line), `<name>` will be replaced by person's name
 * `bodies` — message bodies, separated by `#` symbol, `<name>` will be replaced by person's name

then run:
```bash
./goofmail path/to/files
```

## SMTP

As for now, SSL/TLS connection is default.

## Proxy

As for now, only SOCKS supported (you could use Tor). No auth supported.

## TODO

- Proxy auth and it's http support.

- Provide way to skip proxy.

- Proxy checker.

- Parallelize?

- More backends?

## Build

```bash
sudo apt install cmake libcurl4-openssl-dev

mkdir build; cd build
cmake ..
make
```

## License

Look at COPYING file
