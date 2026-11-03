"""Generate Windows PiPL metadata for K3D Test (little-endian)."""
from pathlib import Path
import struct

def u32(n): return struct.pack('<I', n)
def pstring(s):
    b = s.encode('ascii')
    b = bytes([len(b)]) + b
    return b + b'\0' * (-len(b) % 4)
def cstring(s):
    b = s.encode('ascii') + b'\0'
    return b + b'\0' * (-len(b) % 4)
def prop(key, data):
    return b'8BIM'[::-1] + key.encode('ascii')[::-1] + u32(0) + u32(len(data)) + data

props = [
    prop('kind', b'eFKT'[::-1]),
    prop('name', pstring('K3D Test')),
    prop('catg', pstring('K3D')),
    prop('8664', cstring('EffectMain')),
    prop('ePVR', struct.pack('<HH', 2, 0)),
    prop('eSVR', struct.pack('<HH', 13, 28)),
    prop('eVER', u32((1 << 19) | 1)),
    prop('eINF', u32(0)),
    prop('eGLO', u32(0)),
    prop('eGL2', u32(0)),
    prop('eMNA', pstring('K3D Test Effect')),
    prop('aeFL', u32(0)),
]
Path('pipl.bin').write_bytes(b'\x01\x00' + u32(0) + u32(len(props)) + b''.join(props))
