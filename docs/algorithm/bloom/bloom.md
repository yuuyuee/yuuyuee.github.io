# Bloom Filter

## Standard Bloom Filter

bpe - bits per elements

$$m=-nlnp/(ln2)^2$$
$$bpe=m/n=-lnp/(ln2)^2$$
$$k=ln2*bpe=ln2*m/n=ln2*-lnp/(ln2)^2$$

Optimal number of bits: $bits = (n * ln(p)) / ln(2) ^ 2$
Optimal number of hash functions: $hashes = bpe * ln(2)$

## Cuckoo Filter

## Overview

$e$ - target false positive rate
$f$ - fingerprint length in bits
$a$ - load factor (0 <= a <= 1)
$b$ - number of entries per bucket
$m$ - number of bucket
$n$ - number of items
$C$ - average bits per item

A cuckoo filter is a compact variant of a cuckoo hash table that stores only fingerprints. A set membership query for item $x$ simply searches the hash table for the fingerprint of $x$, and returns true if an identical fingerprint is found.

When constructing a cuckoo filter, its fingerprint size is determined by the target false positive rate $e$. Smaller values of $e$ require longer fingerprints to reject more false queries.

Four major advantages:

1. Supports adding and removing items dynamically
2. Provides higher performance than traditional Bloom Filter, even close to full (e.g. 95% space utilized)
3. Easier to implement
4. Uses less than Bloom Filter if the target false positive rate $e$ is less than 3%
$TPR = TP / (TP + FN)$
$TNR = TN / (TN + FP)$
$FPR = FP / (FP + TN)$
$FNR = FN / (FN + TP)$

A basic cuckoo hash table consists of an array of buckets where each item has two candidate buckets determined by hash functions h1 (x) and h2 (x). Most practical implementations of cuckoo hashing using buckets that hold multiple items.

The lookup procedure checks both buckets to see if either contains this item.

Inserting a new item x in to a hash table, If either of x’s two buckets is empty, the algorithm inserts x to that free bucket and the insertion completes. If neither bucket has space, the item selects one of the candidate buckets, kicks out the existing item and re-inserts this victim item to its own alternate location. This procedure may repeat until a vacant bucket is found. If no vacant bucket is found,this hash table is considered too full to insert. Although cuckoo hashing may execute a sequence of displacements, its amortized insertion time is $O(1)$.

![cuckoo hashing](cuckoo_hash.png)

$f = fingerprint(x)$
$h1(x) = hash(x)$
$h2(x) = h1(x) \bigoplus hash(f)$


![Load factor](load_factor.png)

The fingerprint size must be $f = ceill(log(n)/b)$ bits.

The upper bound of the total probability of a false fingerprint hit is $1-(1-1/{2^f})^{2b} \approx 2b/2^f$

Minimal fingerprint size required is approximately $f \ge ceil(log2(2b/e)) = ceil(log2(1/e) + log2(2b))$

![Space and lookup cost of Bloom filters and three cuckoo filters](space_and_lookup_cost.png)

![Amortized space cost per item vs. measured false positive rate](amortized_space_cost_per_item_and_false_positive_rate.png)

The space-optimal bucket size depends on the target false positive rate $e$: when $e$ > 0.002, having two entries per bucket yields slightly better results than using four entries per bucket; when $e$ decreases to 0.00001 < $e$ ≤ 0.002, four entries per bucket minimizes space.

In summary, we choose (2, 4)-cuckoo filter (i.e., each item has two candidate buckets and each bucket has up to four fingerprints) as the default configuration, because it achieves the best or close-to-best space efficiency for the false positive rates that most practical applications may be interested in.

## Insert

```plaintext
Insert(x):
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

## Lookup

```plaintext
Lookup(x)
  f = fingerprint(x)
  i1 = hash(x)
  i2 = i1 xor hash(f)
  if bucket[i1] or bucket[i2] has f then
    return True;
  return False;
```

## Delete

```plaintext
Delete(x)
  f = fingerprint(x)
  i1 = hash(x)
  i2 = i1 xor hash(f)
  if bucket[i1] or bucket[i2] has f then
    remove a copy of f from this bucket;
    return True;
  return False;
```
