# Bloom Filter

## Standard Bloom Filter

bpe - bits per elements

$$m=-nlnp/(ln2)^2$$
$$bpe=m/n=-lnp/(ln2)^2$$
$$k=ln2*bpe=ln2*m/n=ln2*-lnp/(ln2)^2$$

## Cuckoo Filter

## Overview

A cuckoo filter is a compact variant of a cuckoo hash table that stores only fingerprints. A set membership query for item $x$ simply searches the hash table for the fingerprint of $x$, and returns true if an identical fingerprint is found.

When constructing a cuckoo filter, its fingerprint size is determined by the target false positive rate $e$. Smaller values of $e$ require longer fingerprints to reject more false queries.

Four major advantages:

1. Supports adding and removing items dynamically
2. Provides higher performance than traditional Bloom Filter, even close to full (e.g. 95% space utilized)
3. Easier to implement
4. Uses less than Bloom Filter if the target false positive rate $e$ is less than 3%

A basic cuckoo hash table consists of an array of buckets where each item has two candidate buckets determined by hash functions h1 (x) and
h2 (x). Most practical implementations of cuckoo hashing using buckets that hold multiple items.

The lookup procedure checks both buckets to see if either contains this item.

Inserting a new item x in to a hash table, If either of x’s two buckets is empty, the algorithm inserts x to that free bucket and the insertion completes. If neither bucket has space, the item selects one of the candidate buckets, kicks out the existing item and re-inserts this victim item to its own alternate location. This procedure may repeat until a vacant bucket is found. If no vacant bucket is found,this hash table is considered too full to insert. Although cuckoo hashing may execute a sequence of displacements, its amortized insertion time is $O(1)$.

## Insert

```plaintext
Insert(x)
f = fingerprint(x)
i1 = hash(x)
i2 = i1 xor hash(f)
if bucket[i1 ] or bucket[i2 ] has an empty entry then
  add f to that bucket;
  return Done;

// must relocate existing items;
i = randomly pick i1 or i2 ;
for n = 0; n < MaxNumKicks; n++ do
  randomly select an entry e from bucket[i];
  swap f and the fingerprint stored in entry e;
  i = i xor hash( f );
  if bucket[i] has an empty entry then
    add f to bucket[i];
    return Done;

// Hashtable is considered full;
return Failure;
```
