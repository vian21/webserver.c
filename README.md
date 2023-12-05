# webserver.c

A webserver implementation in C. This project was made while learning about data modeling in C, networking and real-time programming.

### Stress testing

```sh
autocannon http://localhost:5000/ -d 10 -c 30 -w 3
```

- Stress testing with 3 threads, 10 concurrent connectionsfor 30 seconds

## Single thread autocannon resulst

| `stat`    | 2.5%  | 50%     | 97.5%   | 99%     | Avg        | Stdev      | Max     |
| --------- | ----- | ------- | ------- | ------- | ---------- | ---------- | ------- |
| `Latency` | 86 ms | 1605 ms | 5361 ms | 5917 ms | 1848.19 ms | 1436.42 ms | 6738 ms |

| Stat        | 1%      | 2.5%    | 50%     | 97.5%  | Avg    | Stdev   | Min     |
| ----------- | ------- | ------- | ------- | ------ | ------ | ------- | ------- |
| `Req/Sec `  | 2369    | 2369    | 7591    | 8083   | 6504.8 | 2001.28 | 2369    |
| `Bytes/Sec` | 1.35 MB | 1.35 MB | 4.32 MB | 4.6 MB | 3.7 MB | 1.14 MB | 1.35 MB |

- \# of samples: 30
- 130k requests in 10.03s, 37 MB read
- 11 errors (0 timeouts)

## Create a new thread per connection

| `stat`    | 2.5%  | 50%     | 97.5%   | 99%     | Avg        | Stdev      | Max     |
| --------- | ----- | ------- | ------- | ------- | ---------- | ---------- | ------- |
| `Latency` | 60 ms | 1692 ms | 5687 ms | 6306 ms | 1960.92 ms | 1587.26 ms | 7423 ms |

| `Stat`       | 1%      | 2.5%    | 50%     | 97.5%   | Avg     | Stdev   | Min     |
| ------------ | ------- | ------- | ------- | ------- | ------- | ------- | ------- |
| `Req/Sec `   | 3309    | 3309    | 6743    | 7987    | 6232.4  | 1724.29 | 3309    |
| `Bytes/Sec ` | 1.88 MB | 1.88 MB | 3.84 MB | 4.55 MB | 3.55 MB | 981 kB  | 1.88 MB |

- \# of samples: 30
- 125k requests in 10.02s, 35.5 MB read
- 12 errors (0 timeouts)

## Result

- Spawning a new thread each time a connection is made, is a very bad idea.
- as the number of threads increases, the more time it will take for the OS to spawn new one. There is a max of threads you can spawn, and spawning a new thread is expensive
