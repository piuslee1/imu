
# coding: utf-8

# In[1]:


import serial
import re
import redis
import pyquaternion as pyq


# In[2]:


port = '/dev/cu.usbmodem14101'


# In[3]:


s = serial.Serial(port,115200)
r = redis.Redis(host='localhost', port=6379, db=0)


# In[ ]:


while True:
    line = str(s.readline())
    rec = None
    name = None
    if ("Accel" in line):
        rec = re.findall("-?\d+\.\d+", line)
        name = 'acceleration'
    if ("Mag" in line):
        rec = re.findall("-?\d+\.\d+", line)
        name = 'orientation'
    if ("Gyro" in line):
        rec = re.findall("-?\d+\.\d+", line)
        name = 'rot_v'
    if (rec is not None):
        pub = rec[0] + ' ' + rec[1] + ' ' + rec[2]
        r.set(name, pub)
    
    
    #     if "b'" in line:
#         quaternion_values = re.findall("-?\d+\.\d+",line)
#         read_line = (quaternion_values[0] + ' ' 
#                     + quaternion_values[1] + ' ' 
#                     + quaternion_values[2] + ' ' 
#                     + quaternion_values[3])
#         r.set('imu_q',read_line)

