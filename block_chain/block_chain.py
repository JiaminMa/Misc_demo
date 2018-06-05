#!/usr/bin/python
import hashlib as hasher
import datetime as date

class Block:

    def __init__(self, index, timestamp, data,
            previous_hash):
        self.index = index
        self.timestamp = timestamp
        self.data = data
        self.previous_hash = previous_hash
        self.hash = self.hash_block()

    def hash_block(self):
        sha = hasher.sha256()
        sha.update(str(self.index) + str(self.timestamp) + str(self.data) + str(self.previous_hash))
        return sha.hexdigest()

def create_genesis_block():
    return Block(0, date.datetime.now(), "Genesis Block", '0')

def next_block(last_block):
    this_index = last_block.index + 1
    this_time = date.datetime.now()
    this_data = "hello I am blockchain " + str(this_index)
    this_previous_hash = last_block.hash
    return Block(this_index, this_time, this_data, this_previous_hash)

def main():
    blockchain = [create_genesis_block()]
    previous_block = blockchain[0]
    num_of_block_to_add = 20
    for i in range(0, num_of_block_to_add):
        block_to_add = next_block(previous_block)
        blockchain.append(block_to_add)
        previous_block = block_to_add
        print 'Block #{} has been added to the blockchain'.format(i)
        print 'Hash: {}\n'.format(block_to_add.hash)

main()
