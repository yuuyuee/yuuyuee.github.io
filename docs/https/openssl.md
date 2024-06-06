# Openssl

## Generate private key

```bash
openssl genpkey -algorithm RSA -out private.pem -pkeyopt rsa_keygen_bits:2048
```

## Generate public key

```bash
openssl rsa -in private.pem -pubout -out public.pem
```

## View key

```bash
openssl rsa -in private.perm -modulus -text -noout
openssl rsa -pubin -in public.perm -modulus -text -noout
```

## Generate HTTPS cert

```bash
# generating private key
openssl genrsa -out key.pem 2048

# generating certificate signing request
openssl req -new -key key.pem -out csr.pem

# generating self-signed certificate
openssl x509 -req -days 365 -in csr.pem -signkey key.pem -out cert.pem
```
