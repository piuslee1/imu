import numpy as np
import struct

CONVERSIONS = []

FUSION_READ_FORMAT = [
        "accel_x", "accel_y", "accel_z", 
        "gyro_x", "gyro_y", "gyro_z", 
        "mag_x", "mag_y", "mag_z",
        "quat_w", "quat_x", "quat_y", "quat_z",
        "yaw", "pitch", "roll",
        "grav_dir_x", "grav_dir_y", "grav_dir_z", 
        "linear_x", "linear_y", "linear_z",
        "temp", "pressure", "altitude",
]

FUSION_READ_UNITS = [
        "mg", "mg", "mg",
        "degrees/s", "degrees/s", "degrees/s",
        "mG", "mG", "mG", 
        "q", "q", "q", "q",
        "degrees", "degrees", "degrees",
        "mg", "mg", "mg",
        "mg", "mg", "mg",
        "C", "mbar", "feet",
]

DEF_OUT_UNITS = [
        "mg", "mg", "mg",
        "degrees/s", "degrees/s", "degrees/s",
        "mG", "mG", "mG", 
        "q", "q", "q", "q",
        "degrees", "degrees", "degrees",
        "mg", "mg", "mg",
        "mg", "mg", "mg",
        "C", "mbar", "feet",
]

DEF_OUT_FORMAT = [
        None,
        None,
        "linear_x",
        None,
        None,
        "linear_y",
        None,
        None,
        "linear_z",
        "roll",
        "gyro_x",
        "pitch",
        "gyro_y",
        "yaw",
        "gyro_z",
]

def parse_raw(dat):
    # We assume that all data has type float
    return struct.unpack('f'*len(FUSION_READ_FORMAT), dat)

def convert(dat, read_units, out_units=DEF_OUT_UNITS):
    #TODO if unit conversions are need
    if not (read_units == out_units).all():
        for i in len(dat):
            if not read_units[i] == read_units[i]:
                dat[i] = CONVERSIONS[read_units[i]][out_units[i]](dat[i])
    return dat
    
def dformat(dat, read_format, out_format=DEF_OUT_FORMAT):
    out = []
    for f in out_format:
        try:
            out.append(float(dat[read_format.index(f)]))
        except ValueError:
            out.append(float(0))
    return out

def encode(dat):
    return struct.pack('f'*len(dat), *dat)

if __name__ == "__main__":
    # Run test
    dat = encode([float(i) for i in range(len(FUSION_READ_FORMAT))])
    print("Raw bytes: {}".format(dat))
    p = parse_raw(dat)
    print("Parsed data: {}".format(p))
    f = dformat(p, FUSION_READ_FORMAT)
    print("Output data: {}".format(f))
    print("Encoded: {}".format(str(encode(f))))

