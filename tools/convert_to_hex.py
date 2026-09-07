#!/usr/bin/env python3
import sys
def main():
    if len(sys.argv) < 2:
        print("Usage: convert_to_hex.py <binary_file>")
        sys.exit(1)
    with open(sys.argv[1], 'rb') as f:
        data = f.read()
    hex_str = data.hex().upper()
    lines = [hex_str[i:i+64] for i in range(0, len(hex_str), 64)]
    lua_str = '"' + '"\n"'.join(lines) + '"'
    print(lua_str)
if __name__ == "__main__":
    main()
