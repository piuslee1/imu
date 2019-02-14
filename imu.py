import argparse
import redis
import yaml
import serial
import time
import os
import calibrate
import parse

parser = argparse.ArgumentParser(description="IMU driver")
parser.add_argument('--port', help='Serial port for IMU')
parser.add_argument('--baudrate', help='Serial baudrate')
parser.add_argument('--calibration', help='Load calibration file', default=None)
parser.add_argument('--redis_host', help='Redis host server ip')
parser.add_argument('--redis_port', help='Redis server port', default=6379)
parser.add_argument('--config', help='Config for all args')
parser.add_argument('--imu_topic', help='Redis topic to publish data on', default='/imu')
#parser.add_argument('--calibrate', help='Run calibration', default=None)

def parse_args(parser):
    args = parser.parse_args()
    if args.config:
        data = yaml.load(args.config)
        delattr(args, 'config')
        arg_dict = args.__dict__
        for key, value in data.items():
            if isinstance(value, list):
                for v in value:
                    arg_dict[key].append(v)
            else:
                arg_dict[key] = value
    return args

args = parse_args(parser)
ser = serial.Serial(args.port, args.baudrate)
time.sleep(0.1) # BUGS BEGONE. Do not remove
r = redis.Redis(host=args.redis_host, port=args.redis_port, db=0)
calibrator = calibrate.load(args.calibration) # currently identity

while True:
    byte_dat = ser.readline()
    num_dat = parse.convert(parse.parse_raw(byte_dat),
                            parse.FUSION_READ_UNITS,
                            parse.DEF_OUT_UNITS)
    out_dat = parse.dformat(num_dat, parse.FUSION_READ_FORMAT, parse.DEF_OUT_UNITS)
    out_dat = calibrator(out_dat)
    r.publish(args.imu_topic, parse.encode(out_dat))
