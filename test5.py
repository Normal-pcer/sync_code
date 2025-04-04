def decode_e(e):
    for b in range(16):
        a = (16*(b+1) - e) % 256
        if 0 <= a <= 15:
            return (a << 4) | b
    return 0

def find_key(data):
    for L in range(1, 256):
        pos = len(data) - (L + 1)
        if pos < 0:
            continue
        if data[pos] == L:
            encoded_key = data[pos+1 : pos+1+L]
            key = bytearray()
            for e in reversed(encoded_key):
                key.append(decode_e(e))
            return bytes(key), data[:pos]
    return None, None

def decrypt(data, key):
    s = list(range(256)) + [0, 0]
    c = 0
    # KSA
    for i in range(256):
        c = (s[i] + c + key[i % len(key)]) % 256
        s[i], s[c] = s[c], s[i]
    # PRGA
    s256, s257 = 256, 257
    for i in range(len(data)):
        s[s256] = (s[s256] + 1) % 256
        s[s257] = (s[s257] + s[s[s256]]) % 256
        a, b = s[s256], s[s257]
        s[a], s[b] = s[b], s[a]
        t = (s[a] + s[b]) % 256
        data[i] ^= s[t]
    return data

def main():
    with open('enc.bin', 'rb') as f:
        data = bytearray(f.read())
    key, encrypted_data = find_key(data)
    if not key:
        print("Key not found")
        return
    decrypted_data = decrypt(encrypted_data, key)
    with open('decrypted.bin', 'wb') as f:
        f.write(decrypted_data)
    print("Decrypted data written to decrypted.bin")

if __name__ == '__main__':
    main()