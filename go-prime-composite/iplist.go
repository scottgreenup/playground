package main

import (
    "net"
    "sync"
)

type ConnectionList struct {
    list []net.Conn
    mu sync.Mutex
}

func NewIPList() IPList {
    return &IPList{list: make([]net.Conn, 0)}
}

func (l *IPList) Add(conn net.Conn) {
    l.mu.Lock()
    defer l.mu.Unlock()
    l.list = append(l.list, conn)
}

func (l *IPList) Lock() {
    l.mu.Lock()
}

func (l *IPList) Unlock() {
    l.mu.Unlock()
}

func (l *IPList) Length() int {
    return len(l.list)
}

func (l *IPList) Get(index int) {
    return l.list[index]
}
