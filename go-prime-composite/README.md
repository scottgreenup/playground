
# General
 - Should create a puppet/chef instance that can be deployed easily.
 - Use a deployment script to deploy the servers with configuration.

# Configuration File
The configuration file contains a list of the IP addresses that will be running
the program. The top of the list will be the dealer; the others will be workers.

# Worker-Dealer system
The idea is that there is one dealer who deals work to the workers. If possible,
the dealer will also do some of the work.

  - This amount of work should be proportional to the amount of workers
  - i.e. the more workers there are, the less work the dealer does.

## Requirements
 - Dealer is originally selected from ordering of IPs in configuration.
 - Workers elect leader based upon ID, lowest ID wins; ID comes from list in cfg
 - Implement a master-election system.
 - Dealer that comes back online should ask everyone who is dealer; if no one
   it should let everyone know it is the dealer

```
ip_list = [x for x in open(config_filename).read()]
conns = [conn(ip) for ip in ip_list]

if my_ip == ip_list[0] then
  put_hand_up_for_dealer()
else
  wait_for_dealer()
```

## Selecting the Dealer

 - The dealer will contact everyone
 - If a worker doesn't hear from the dealer, they'll waiting their ID * 30 seconds
   to become dealer themselves.
