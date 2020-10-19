import pandas as pd
from tqdm import tqdm



class Node(object):
   def __init__(self) -> None:
       self.children = {}
       self.value = None


def find(node, key):
    for c in key:
        if c not in node.children:
            return False
        node = node.children[c]
    return node.value


def insert(node, key, value):
    for c in key:
        if c not in node.children:
            node.children[c] = Node()
        node = node.children[c]
    node.value = value


def count_index(node, count):
    if node.value == "Done":
        count += 1
        node.value = count

    if node.children != {}:
        for child in node.children:
            count = count_index(node.children[child], count)

    return count


def convert_ip(ip):
    ip, network_length = ip.split('/')[0], int(ip.split('/')[1])
    network = ''.join(list(map(lambda i: str(format(int(i), '08b')), ip.split('.'))))
    if len(network) >= network_length:
        return network[:network_length]
    else:
        return network.ljust(network_length, '0')


def main():
    count = 0

    with open("route-02.txt", "r") as f:
        ips = f.readlines()
        ips = list(map(lambda ip: ip.replace('\n', ''), ips))
        ips = list(map(lambda ip: convert_ip(ip), ips))

    node = Node()
    for ip in tqdm(ips):
        insert(node, ip, "Done")
    
    count_index(node, count)
    
    targets = ["41.74.1.1", "66.31.10.3", "133.5.1.1", "209.143.75.1", "221.121.128.1"]

    for target in targets:
        for i in range(32):
            network_length = f"/{32 - i}"
            idx = find(node, convert_ip(target+network_length))
            if (idx != False) and (idx != None):
                break
        # answer
        print(target, idx)


if __name__ == "__main__":
    main()