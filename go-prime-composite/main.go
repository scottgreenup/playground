package main

import (
    "bufio"
    "flag"
    "fmt"
    "io/ioutil"
    "math"
    "net"
    "strings"
    "strconv"
    "sync"
    "time"
)

var task_id = flags.Int("task_id", -1, "ID is the task's identifier, matches\
the list of IPs given on -config.")
var port = flags.Int("port", 8080, "The port to listen for work.")
var config_filename = flag.String(
    "config", "config", "The config file; currently just a list of IPs")

func isPrime(x uint64) bool {
    root := uint64(math.Sqrt(float64(x))) + 1
    for i := uint64(2); i <= root; i++ {
        if (x % i == 0) {
            return false
        }
    }
    return true
}

// manager produces primes for the workers by putting it on ch
// comp - The composite we are trying to find the factors of
// ch - The channel on which the workers are listening
// TODO(greenup): implement prime sieve
func manager(comp uint64, ch chan uint64, answer chan uint64) {
    defer close(ch)
    ch <- 2
    for curr := uint64(3); curr < comp; curr += 2 {
        select {
        case a := <-answer:
            b := <-answer
            fmt.Printf("%d * %d\n", a, b);
            return
        default:
            if isPrime(curr) {
                ch <- curr
            }
        }
    }
}

// worker recieves primes on ch and divides comp by this prime, if the answer is
// also a prime, we therefore have the 2 prime factors
// comp - The composite we are trying to find the factors of
// ch - The channel on which the workers are listening
func worker(comp uint64, ch chan uint64, answer chan uint64, answer_mu* sync.Mutex) {
    for {
        prime, more := <-ch
        if more {
            divided := float64(comp) / float64(prime);
            if math.Floor(divided) != divided {
                continue
            }

            if isPrime(uint64(divided)) {
                answer_mu.Lock()
                answer <- uint64(divided)
                answer <- prime
                answer_mu.Unlock()
                return
            }
        } else {
            return
        }
    }
}

type WorkBit struct {
    factors []uint64
    composite uint64
}

func ReadConfiguration(filename string) []*net.TCPAddr {
    addresses := make([]*net.TCPAddr, 0)
    input, err := ioutil.ReadFile(filename)
    if err != nil {
        fmt.Println(err.Error())
        return nil
    }

    lines := strings.Split(string(input), "\n");
    for i := 0; i < len(lines); i++ {
        line := lines[i]
        if len(line) == 0 {
            continue
        }
        addr, err := net.ResolveTCPAddr("tcp", line)
        if err != nil {
            fmt.Println(err.Error())
            break
        }
        addresses = append(addresses, addr)
        fmt.Println(addr)
    }

    return addresses
}

func ListenForDealer(port int, conn_ch chan net.Conn) {
    addr, err := net.ResolveTCPAddr("tcp", ":" + strconv.Itoa(port));
    if err != nil {
        fmt.Println(err.Error())
        return
    }

    fmt.Printf("Listening on %s:%d\n", addr.IP, addr.Port)
    l, err := net.ListenTCP("tcp", addr);
    if err != nil {
        fmt.Println(err.Error())
        return
    }

    for {
        conn, err := l.Accept()
        if err != nil {
            continue
        }
        conn_ch <- conn
        break
    }
}

// CreateWork is the function that generates work that will be handed out to the
// workers through the DealWork function.
// TODO work out how to stop this guy early and cleanly, I hate using another ch
func CreateWork(work_ch chan WorkBit) {
    defer close(work_ch)
    comp := uint64(15485867) * uint64(15485863) // 6201559691
    primes := make([]uint64)
    primes = append(factors, 2)

    for curr := uint64(3); curr < comp; curr += 2 {
        if len(primes) == 10 {
            work_ch <- &WorkBit{factors: primes, composite: comp}
            primes = make([]uint64)
        }
    }
}

func DealWork(hostname string, id int, work_ch chan WorkBit) {
    conn, err := net.Dial("tcp", hostname)
    if err != nil {
        fmt.Println(err.Error())
        return
    }

    writer := bufio.NewWriter(conn)

    // Send the ID of the dealer through first.
    writer.WriteString(strconv.Itoa(id) + "\n")

    for {
        // TODO deal with how this thing will close
        work := <-work_ch
        // TODO serialize work and send it off
    }

}

// Forever listens for a connection on `port`, if it recieves an answer for the
// composite number it will test the value and if it is true it will finish.
func ListenForAnswer(port int, composite uint64) {
    addr, err := net.ResolveTCPAddr("tcp", ":" + strconv.Itoa(port));
    if err != nil {
        fmt.Println(err.Error())
        return
    }

    fmt.Printf("Listening on %s:%d\n", addr.IP, addr.Port)
    l, err := net.ListenTCP("tcp", addr);
    if err != nil {
        fmt.Println(err.Error())
        return
    }

    for {
        conn, err := l.Accept()
        if err != nil {
            continue
        }

        // TODO this may not be syntactically correct
        go func() {
            reader := bufio.NewReader(conn)
            line, err := reader.ReadString('\n')
            if err != nil {
                break
            }
            line = line[:(len(line)-1)]
            answer, err := uint64(strconv.Atoi(line))
            if err != nil {
                fmt.Println(err.Error())
                break
            }

            if composite % answer == 0 {
                fmt.Printf(
                    "%d * %d = %d\n", answer, composite/answer, composite);
            }
        }
    }
}

func GetDealerId(conn net.Conn) (int, error) {
    reader := bufio.NewReader(conn)
    line, err := reader.ReadString('\n')
    if err != nil {
        return 0, err
    }
    line = line[:(len(line)-1)]
    id, err := uint64(strconv.Atoi(line))
    if err != nil {
        return 0, err
    }
    return id, nil
}

func main() {
    flag.Parse()

    if task_id == -1 {
        fmt.Println("Please set ID with -task_id.")
        return 0
    }

    // Read the configuration file in; it is currently just a list of IPs
    // TODO transform the config file into a JSON file for extensibility
    config := ReadConfiguration(*config_filename)

    // listen() for a dealer in another thread
    conn_ch := make(chan net.Conn)
    go ListenForDealer(port, conn_ch)

    // At some interval of 30 seconds we will get a connection from a task
    // that decided to be a dealer; otherwise we are the dealer and start
    // dealing out work
    select {
        case <-time.After(task_id * 30 * time.Second):
            // Start creating work to be dealt to workers
            work_ch := make(chan WorkBit, 10)
            go CreateWork(work_ch)

            // For each worker, creat a connection and give them a chunk of
            // work when they ask for it.
            for i := 0; i < len(config); i++ {
                if i != task_id {
                    go DealWork(config[i], task_id, work_ch)
                }
            }
            answer := ListenForAnswer(port)

        case read_conn := <-conn_ch:

            // The first thing read_conn will tell us is the task_id of who
            // connected
            dealer_id, err := GetDealerId(read_conn)

            if err != nil {
                fmt.Println(err.Error())
                return 0
            }

            work_ch := make(chan WorkBit)
            answer_ch := make(chan uint64)
            go RecieveWork(read_conn, work_ch) // TODO
            go ProcessWork(work_ch, answer_ch) // TODO
            go ProcessWork(work_ch, answer_ch)
            go ProcessWork(work_ch, answer_ch)
            go ProcessWork(work_ch, answer_ch)

            // If we are given an answer, connect to the dealer
            InformDealer(dealer_id, answer_ch) // TODO
    }


    select {}
    //comp := 6201559691
    //comp := uint64(15485867) * uint64(15485863)
    //ch := make(chan uint64)
    //answer := make(chan uint64)
    //var answer_mu sync.Mutex

    //go worker(comp, ch, answer, &answer_mu)
    //go worker(comp, ch, answer, &answer_mu)
    //go worker(comp, ch, answer, &answer_mu)
    //go worker(comp, ch, answer, &answer_mu)
    //manager(comp, ch, answer)
}
